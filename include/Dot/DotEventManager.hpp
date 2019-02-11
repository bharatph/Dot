#ifndef _DOT_EVENT_MANAGER
#define _DOT_EVENT_MANAGER
#include <Dot/DotEvent.hpp>
#include <em/EventManager.hpp>
#include <Dot/Dot.hpp>

namespace dot
{
  class Dot;
class DotEventManager : public em::EventManager<dot::DotEvent, Dot &>
{
public:
  DotEventManager()
  {
  }

  DotEventManager &chain(DotEvent dotEvent, EventCallback eventCallback)
  {
    on(dotEvent, eventCallback);
    return (*this);
  }

  DotEventManager &onConnect(EventCallback eventCallback)
  {
    return chain(DotEvent::CONNECTED, eventCallback);
  }

  DotEventManager &onDisconnect(EventCallback eventCallback)
  {
    return chain(DotEvent::DISCONNECTED, eventCallback);
  }

  DotEventManager &onFailed(EventCallback eventCallback)
  {
    return chain(DotEvent::FAILED, eventCallback);
  }
};
} // namespace dot
#endif
