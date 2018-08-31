#ifndef _DOT_OP_EVENT_MANAGER_H
#define _DOT_OP_EVENT_MANAGER_H

#include <em/EventManager.hpp>
#include <Dot/DotOperationEvent.hpp>
#include <Dot/Dot.hpp>

namespace dot {
class DotOperation;

class DotOperationEventManager : protected em::EventManager<DotOperationEvent, Dot &> {
    public:
    DotOperationEventManager &addDotOperationEventHandler(DotOperationEvent DotOperationEvent, EventCallback eventCallback){
        addEventHandler(DotOperationEvent, eventCallback);
        return *this;
    }
};
}
#endif
