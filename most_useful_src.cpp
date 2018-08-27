#include <iostream>
#include <functional>
#include <future>
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


class Dot : public em::EventManager<DotEvent, const DotState &> {
    public:
    class DotDetails {
        public:
        DotDetails(){
            
        }
    };
    Dot(){
        
    }
    Dot(const Dot &dot){
        
    }
    void connect(std::string host, int port){
        DotState dotState;
        dotState.val = "connected";
        fireEvent(DotEvent::CONNECTED, dotState);
    }
    void disconnect(){
        //TODO if connected disconnect, else don't bother
        DotState dotState;
        dotState.val = "disconnected";
        fireEvent(DotEvent::DISCONNECTED, dotState);
    }
    void write(std::string message, std::function<void(DotState &)> writeFunc){
        DotState dotState;
        writeFunc(dotState);
    }
    void read(std::function<void(DotState &)> readFunc){
        DotState dotState;
        readFunc(dotState);
    }
};

class LightDot : public Dot {
    public:
    int f(){
        return 0;
    }
    LightDot(){
    addEventHandler(DotEvent::CONNECTED, [](const DotState &dotState){
        std::cout << dotState.val << std::endl;
    });
    addEventHandler(DotEvent::DISCONNECTED, [](const DotState &dotState){
        std::cout << dotState.val << std::endl;
    });
    write("hello", [](const DotState &dotState){
        std::cout << "me: hello" << std::endl;
    });
    read([](const DotState &dotState){
        std::cout << dotState.val << std::endl;
    });
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
    lightDot.connect("localhost", 22);
    lightDot.disconnect();
    }
};
//solo.cpp
int main(int argc, char *argv[]) {
    SOLO solo;
   return 0;
}