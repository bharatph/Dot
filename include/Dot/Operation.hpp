#ifndef _DOT_OPERATION_H
#define _DOT_OPERATION_H

#include <em/EventManager.hpp>
#include <Dot/DotOperationEvent.hpp>

namespace dot
{
class Dot;
class Operation : public em::EventManager<DotOperationEvent, Dot &, std::string>
{

public:
  typedef std::function<void(Operation &)> OperationCallback;

private:
  Operation &chain(DotOperationEvent, EventCallback);
  Dot *d;
  OperationCallback operationCallback;

public:
  Operation(Dot &d, OperationCallback);
  ~Operation();
  Dot &getDot();
  Operation &onSuccess(EventCallback);
  Operation &onFailed(EventCallback);
  void execute();
};
} // namespace dot

#endif
