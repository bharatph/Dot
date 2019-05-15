#include <Dot/Operation.hpp>
#include <Dot/DotOperationEvent.hpp>

dot::Operation::~Operation()
{
  std::cout << "Ending" << std::endl;
}

dot::Operation::Operation(Dot &d, OperationCallback operationCallback) : d(&d), operationCallback(operationCallback)
{
}

dot::Dot &dot::Operation::getDot()
{
  return *d;
}

dot::Operation &dot::Operation::chain(dot::DotOperationEvent dotOperationEvent, EventCallback eventCallback)
{
  on(dotOperationEvent, eventCallback);
  return *this;
}

dot::Operation &dot::Operation::onSuccess(EventCallback eventCallback)
{
  return chain(DotOperationEvent::SUCCESS, eventCallback);
}

dot::Operation &dot::Operation::onFailed(EventCallback eventCallback)
{
  return chain(DotOperationEvent::FAILED, eventCallback);
}

void dot::Operation::execute(){
  operationCallback(*this);
}
