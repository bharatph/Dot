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

class DotOperationEventManager : protected em::EventManager<DotOperationEvent, DotOperation &> {
    public:
    void addDotOperationEventHandler(DotOperationEvent DotOperationEvent, EventCallback eventCallback){
        addEventHandler(DotOperationEvent, eventCallback);
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

class Writer : public DotOperation{
    public:
    typedef int comm_socket; //TODO REMOVE
    comm_socket sock;
    Writer(comm_socket sock){
        this->sock = sock;
    }
    DotOperation &write(std::string message){
        //int bytes_written = comm_write(sockfd, message);
        //if(bytes_written < 0){
            fireEvent(DotOperationEvent::FAILED, *this);
        //} else {
            fireEvent(DotOperationEvent::SUCCESS, *this);
        //}
        return *this;
    }
};

class Reader : public DotOperation{
    public:
    Reader(){
    }
    void read(){
        //if(error){
            fireEvent(DotOperationEvent::FAILED, *this);
        //} else {
            fireEvent(DotOperationEvent::SUCCESS, *this);
        //}
    }
};

class Dot : public DotEventManager {
    private:
    typedef int comm_socket; //TODO REMOVE
    comm_socket _sock;

    std::map<std::string, EventCallback> readForMap;
    std::queue<DotOperation> incomingQueue;
    std::queue<DotOperation> outgoingQueue;

    std::future<void> runner;

    void _init(){
        //initialize socket structure
    }
    void _runnerLoop(){
        //while(1){
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
        _runnerLoop();
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
        Writer &writer = *(new Writer(_sock));
        outgoingQueue.push(writer.write(message));
        return writer;
    }


    Reader &readFor(std::string message, EventCallback readForCallback){
        Reader *reader = new Reader();
        readForMap[message] = readForCallback;
        //reader->read(message);
        //incoming_queue.push(reader.getOperation());
        return *reader;
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
        _runnerLoop();
    }
    ~Dot(){
        std::cout << "Dot ended" << std::endl;
    }
};

class LightDot : public Dot {
    public:
    LightDot(){
    readFor("hi", [](Dot &dot){
        dot.write("hello");
    });
    readFor("off", [](Dot &dot){
        dot.write("off");
    });
    readFor("on", [](Dot &dot){
        dot.write("on");
    });
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
        .addDotOperationEventHandler(DotOperationEvent::SUCCESS, [](const DotOperation &){
            std::cout << "turned OFF successfully" << std::endl;
        });
    lightDot.connect("localhost", 22);
    lightDot.resume();
    lightDot.disconnect();
    std::cout << "solo" << std::endl;
    }
    public:
    SOLO(){
    }

    int run(){
        //fireEvent()
        //while(1);
    fireEvent(SoloState::START);
        _init();
        return 0;
    }
    ~SOLO(){
        fireEvent(SoloState::EXIT);
    }
};


//solo.cpp
int main(int argc, char *argv[]) {
    SOLO solo;
    solo.addEventHandler(SoloState::START, [](){
        std::cout << "started solo" << std::endl;
    });
    solo.addEventHandler(SoloState::EXIT, [](){
        std::cout << "solo killed" << std::endl;
    });
   return solo.run();
}
