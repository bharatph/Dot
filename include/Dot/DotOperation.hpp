#ifndef _DOT_OPERATION_H
#define _DOT_OPERATION_H

#include <em/EventManager.hpp>
#include <Dot/DotOperationEvent.hpp>

namespace dot
{
class Dot;
class DotOperation : public em::EventManager<DotOperationEvent, Dot &, std::string>
{

public:
  typedef std::function<void(DotOperation &)> OperationCallback;

private:
  DotOperation &chain(DotOperationEvent, EventCallback);
  OperationCallback operationCallback;
  Dot *dot;

public:
  DotOperation(Dot &dot, OperationCallback);
  ~DotOperation();
  Dot &getDot();
  DotOperation &onSuccess(EventCallback);
  DotOperation &onFailed(EventCallback);
  void execute();
};
} // namespace dot

#endif
