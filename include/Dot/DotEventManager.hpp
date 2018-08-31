#ifndef _DOT_EVENT_MANAGER_H
#define _DOT_EVENT_MANAGER_H

#include <em/EventManager.hpp>
#include <Dot/DotEvent.hpp>

namespace dot {
  //declaration of Dot to avoid cyclic errors
class Dot;

class DotEventManager : protected em::EventManager<DotEvent, Dot &> {
    public:
    DotEventManager &addDotEventHandler(DotEvent dotEvent, EventCallback eventCallback){
        addEventHandler(dotEvent, eventCallback);
        return *this;
    }
};
}

#endif
