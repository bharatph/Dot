#ifndef _DOT_OPERATION_H
#define _DOT_OPERATION_H

#include <em/EventManager.hpp>
#include <Dot/DotOperationEvent.hpp>

namespace dot {
  class Dot;
class DotOperation : public em::EventManager<DotOperationEvent, Dot &> {
    public:
    DotOperation();
    ~DotOperation();
    virtual void notify(std::string);
    DotOperation &on(DotOperationEvent, EventCallback);
    EventCallback getEventHandler();
};
}

#endif
