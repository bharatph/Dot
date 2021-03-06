#include <iostream>
#include <functional>
#include <future>
#include <vector>
#include <map>
#include <queue>

namespace em
{
template <class Event, typename... EventCallbackType>
class EventManager
{
  public:
    typedef std::function<void(EventCallbackType...)> EventCallback;
  private:
    std::map<Event, EventCallback> events;

  public:
    EventManager()
    {
    }

    void addEventHandler(Event event, EventCallback callback)
    {
        events[event] = callback;
    }

    template <class T>
    void addEventHandler(Event event, T *const object, void (T::*callback)(EventCallbackType...))
    {
        using namespace std::placeholders;
        events[event] = std::bind(callback, object, _1);
    }

protected:
    void fireEvent(Event event, EventCallbackType... t)
    {
        if (events[event] == nullptr)
            return;
        EventCallback callback = events[event];
        callback(t...);
    }
};
}

//Dot level event
enum class DotEvent {
    CONNECTED,
    RESUME,
    PAUSE,
    DISCONNECTED
};

class Dot;

class DotEventManager : protected em::EventManager<DotEvent, Dot &> {
    public:
    DotEventManager &addDotEventHandler(DotEvent dotEvent, EventCallback eventCallback){
        addEventHandler(dotEvent, eventCallback);
        return *this;
    }
};


/*
class DotProtocol{
    protected:
    bool running = false;
    public:
  DotProtocol() = default;
  virtual void start() = 0;
  virtual void end() = 0;
  virtual void send(std::string) = 0;
  virtual void recv() = 0;
  virtual ~DotProtocol() = default;
};
*/



//Events have states
//success
//failed - decribe failure (timeout? no network? network unreachable?)
//undergoing //can be polled by the user
enum class DotOperationEvent {
    SUCCESS,
    FAILED,
    PROCESSING
};

class DotOperation;

class DotOperationEventManager : protected em::EventManager<DotOperationEvent, Dot &> {
    public:
    DotOperationEventManager &addDotOperationEventHandler(DotOperationEvent DotOperationEvent, EventCallback eventCallback){
        addEventHandler(DotOperationEvent, eventCallback);
        return *this;
    }
};


class DotOperation : public DotOperationEventManager {
    public:
    DotOperation(){

    }
    ~DotOperation(){
        std::cout << "Ending" << std::endl;
    }
};

//sends message, after callback is completed destroys itself
class Writer : public DotOperation{
    public:
    Dot *dot;
    Writer(Dot *dot){
        this->dot = dot;
    }
    DotOperation &write(std::string message){
        //int bytes_written = comm_write(sockfd, message);
        //if(bytes_written < 0){
            //fireEvent(DotOperationEvent::FAILED, *dot, *this);
        //} else {
            //fireEvent(DotOperationEvent::SUCCESS, *dot, *this );
        //}
        return *this;
    }
};

//unlike Writer, Reader does not get destroyed unless explicity mentioned
class Reader : public DotOperation{
    private:
    Dot *dot;
    //get socket from dot
    public:
    Reader(Dot *dot){
        this->dot = dot;
    }
    Reader &read(std::string message){
        //if(error){
            //fireEvent(DotOperationEvent::FAILED, *dot, *this);
        //} else {
            //fireEvent(DotOperationEvent::SUCCESS, *dot, *this);
        //}
        return *this;
    }
};

class Dot : public DotEventManager {
    private:
    typedef int comm_socket; //TODO REMOVE
    comm_socket _sock;

    std::map<std::string, EventCallback> readForMap;
    std::queue<DotOperation> incomingQueue;
    std::queue<DotOperation> outgoingQueue;

    comm_socket getSocket(){
        return _sock;
    }

    std::future<void> runner;

    void _init(){
        //initialize socket structure
    }
    void _readLoop(){
        //while(1){
        //TODO std::string received = comm_read(sock);
        //TODO if received == the following iteration call that function
            std::map<std::string, EventCallback>::iterator it;
            for(it = readForMap.begin(); it != readForMap.end(); ++it){
                DotOperation *dotOperation = new DotOperation();
                it->second(*this);
            }
        //}
    }
    protected:
    Dot(){

    }
    public:
    class DotDetails {
        public:
        DotDetails(){

        }
    };
    Dot(const Dot &dot){

    }
    void connect(std::string host, int port){
        //DotOperation dotOperation;
        //dotState.val = "connected";
        //int connection = comm_connnect(sockfd);
        fireEvent(DotEvent::CONNECTED, *this);
        _readLoop();
    }
    void disconnect(){
        //TODO if connected disconnect, else don't bother
        //DotOperation dotOperation;
        //dotState.val = "disconnected";
        fireEvent(DotEvent::DISCONNECTED, *this);
    }
    void resume(){
        //DotOperation dotOperation;
        //dotState.val = "resume";
        fireEvent(DotEvent::RESUME, *this);
    }
    Writer &write(std::string message){
        //add to queue and let runner decide whether to run or not
        //DotState dotState;
        //writeFunc(dotState);
        Writer &writer = *(new Writer(this));
        outgoingQueue.push(writer.write(message));
        return writer;
    }

    Reader &readFor(int binaryFile, std::string fileType){
        Reader &reader = *(new Reader(this));
        //readForMap[fileType] = readForCallback;
        return reader.read(fileType);
    }

    Reader &readFor(std::string message){
        Reader &reader = *(new Reader(this));
        //readForMap[message] = readForCallback;
        return reader.read(message);
    }
    /*
    Reader &readFor(std::vector<std::string> messages){
        Reader *reader = new Reader();
        for(std::string message : messages){
            reads[message] = readFunc;
            reader.read(messages)
        }
        return *reader;
    }
    */

    void run(){
        //runner = std::async(std::launch::async, &Dot::_run, this);
        _readLoop();
    }
    ~Dot(){
        std::cout << "Dot ended" << std::endl;
    }
};

class LightDot : public Dot {
    public:
    LightDot(){
    readFor("hi").addDotOperationEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot){
        dot.write("hello");
    });
    /*
    readFor("off", [](Dot &dot){
        dot.write("off");
    });
    readFor("on", [](Dot &dot){
        dot.write("on");
    });
    */
    }

    DotOperation &on(){
        return write("on");
    }
    DotOperation &off(){
        return write("off");
    }
    DotOperation &setLuminous(int val){
      return write("val+" + val);
    }
};

enum class SoloState {
    START,
    EXIT
};

//SOLO.cpp
class SOLO : public em::EventManager<SoloState> {
    private:
    void _init(){
    LightDot lightDot;
    lightDot.addDotEventHandler(DotEvent::CONNECTED, [](const Dot &dot){
        std::cout << "connected" << std::endl;
    }).addDotEventHandler(DotEvent::DISCONNECTED, [](const Dot &dot){
        std::cout << "disconnected" << std::endl;
    }).addDotEventHandler(DotEvent::PAUSE, [](const Dot &dot){
        std::cout << "pause" << std::endl;
    }).addDotEventHandler(DotEvent::RESUME, [](const Dot &dot){
        std::cout << "resume" << std::endl;
    });

    lightDot.on();
    lightDot.setLuminous(0);
    lightDot.off()
        .addDotOperationEventHandler(DotOperationEvent::SUCCESS, [](const Dot &){
            std::cout << "turned OFF successfully" << std::endl;
        });
    lightDot.connect("localhost", 22);
    lightDot.resume();
    lightDot.disconnect();
    }
    public:
    SOLO(){
    }

    int run(){
    //todo while(1);
    fireEvent(SoloState::START);
        _init();
        return 0;
    }
    ~SOLO(){
        fireEvent(SoloState::EXIT);
    }
};

int exit_handler(int sigevent){
    std::cout << "solo killed" << std::endl;
    exit(sigevent);
}

/*
 * DotProtocol categorizes whether the protocol is stateful or stateless
 */
class DotProtocol{
public:
  DotProtocol();
};

/*
 * DotProtocolConfiguration supports DotProtocol for advance operations
 * often extended and custom functions are defined
 */
class DotProtocolConfiguration(){
public:
  DotProtocolConfiguration();
};

//solo.cpp
int main(int argc, char *argv[]) {
    //register exit event
    SOLO solo;
    solo.addEventHandler(SoloState::START, [](){
        std::cout << "started solo" << std::endl;
    });
    solo.addEventHandler(SoloState::EXIT, [](){
        exit_handler(0);
    });
    Dot &dot = Dot::getDot();
    //other way of initializing Dot
    //default protocol is html
    Dot &dot = Dot::getDot().with(HTML);
    dot.on(DotEvent::CONNECTED, [](Dot &dot){
      //write using http
      //using local dot
      //TODO add support for stateless protocol such as html
      //the write implementation for HTML constructs html with configuration class
      dot.through(DotProtocol).with(DotProtocolConfiguration).write("hi");
      //write as is
      dot.write("hi");
    });
   return solo.run();
}
