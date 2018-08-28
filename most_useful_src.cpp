#include <iostream>
#include <functional>
#include <future>
#include <vector>
#include <map>

namespace em
{
template <class Event, class EventCallbackType>
class EventManager
{
  public:
    typedef std::function<void(EventCallbackType)> EventCallback;

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
    void addEventHandler(Event event, T *const object, void (T::*callback)(EventCallbackType))
    {
        using namespace std::placeholders;
        events[event] = std::bind(callback, object, _1);
    }

    void fireEvent(Event event, EventCallbackType t)
    {
        if (events[event] == nullptr)
            return;
        EventCallback callback = events[event];
        callback(t);
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

//Events have states
//success
//failed - decribe failure (timeout? no network? network unreachable?)
//undergoing //can be polled by the user

enum class DotStateEvent {
    SUCCESS,
    FAILED,
    PROCESSING
};

class DotState : public em::EventManager<DotStateEvent, const DotState &> {
    public:
    std::string val = "idle";
    DotEvent dotEvent = DotEvent::DISCONNECTED;
    DotState(){
    }
};

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


class Dot : public em::EventManager<DotEvent, const DotState &> {
    private:
    std::map<std::string, std::function<void(DotState &)>> reads;
    std::map<std::string, std::function<void(DotState &)>> writes;
    std::future<void> runner;
    void _run(){
        //while(1){
            std::map<std::string, std::function<void(DotState &)>>::iterator it;
            for(it = reads.begin(); it != reads.end(); ++it){
                DotState *dotState = new DotState();
                it->second(*dotState);
            }
        //}
    }
    protected:
    DotState ds;
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
        DotState dotState;
        dotState.val = "connected";
        fireEvent(DotEvent::CONNECTED, dotState);
        _run();
    }
    void disconnect(){
        //TODO if connected disconnect, else don't bother
        DotState dotState;
        dotState.val = "disconnected";
        fireEvent(DotEvent::DISCONNECTED, dotState);
    }
    void write(std::string message, std::function<void(const DotState &)> writeFunc){
        //add to queue and let runner decide whether to run or not
        DotState dotState;
        std::cout << "sending: " << message << std::endl;
        writeFunc(dotState);
    }
    void read(std::string message, std::function<void(const DotState &)> readFunc){
        DotState dotState;
        reads[message] = readFunc;
    }
    void read(std::vector<std::string> messages, std::function<void(const DotState &)> readFunc){
        DotState dotState;
        for(std::string message : messages){
            reads[message] = readFunc;
        }
    }

    void run(){
        //runner = std::async(std::launch::async, &Dot::_run, this);
        _run();
    }
    ~Dot(){
    }
};

class LightDot : public Dot {
    public:
    LightDot(){
    addEventHandler(DotEvent::CONNECTED, [](const DotState &dotState){
        std::cout << dotState.val << std::endl;
    });
    addEventHandler(DotEvent::DISCONNECTED, [](const DotState &dotState){
        std::cout << dotState.val << std::endl;
    });
    read("hi", [](const DotState &dotState){
        std::cout << dotState.val << std::endl;
    });
    read("off", [](const DotState &dotState){
        std::cout << "turnedOff" << std::endl;
    });
    read("on", [](const DotState &dotState){
        std::cout << "turnedOn" << std::endl;
    });
    }
    DotState &on(){
        DotState *dotState = new DotState();
        //write("on", dotState);
        return *dotState;
    }
};

enum class SoloState {
    START,
    EXIT
};

//SOLO.cpp
class SOLO : public em::EventManager<SoloState, int> {
    public:
    SOLO(){
    LightDot lightDot;
        lightDot.on()
            .addEventHandler(DotStateEvent::SUCCESS, [](const DotState &){
            std::cout << "success" << std::endl;
        });/*.addEventHandler(DotStateEvent::FAILURE, [](const DotState &){
            std::cout << "failure" << std::endl;
        });*/
    lightDot.connect("localhost", 22);
    lightDot.disconnect();
    }
};
//solo.cpp
int main(int argc, char *argv[]) {
    SOLO solo;
   return 0;
}
