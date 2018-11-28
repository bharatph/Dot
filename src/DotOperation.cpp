#include <Dot/DotOperation.hpp>
#include <Dot/DotOperationEvent.hpp>

dot::DotOperation::~DotOperation()
{
  std::cout << "Ending" << std::endl;
}

dot::DotOperation::DotOperation(Dot &dot, OperationCallback operationCallback) : dot(&dot), operationCallback(operationCallback)
{
}

dot::Dot &dot::DotOperation::getDot()
{
  return *dot;
}

dot::DotOperation &dot::DotOperation::chain(dot::DotOperationEvent dotOperationEvent, EventCallback eventCallback)
{
  on(dotOperationEvent, eventCallback);
  return *this;
}

dot::DotOperation &dot::DotOperation::onSuccess(EventCallback eventCallback)
{
  return chain(DotOperationEvent::SUCCESS, eventCallback);
}

dot::DotOperation &dot::DotOperation::onFailed(EventCallback eventCallback)
{
  return chain(DotOperationEvent::FAILED, eventCallback);
}

void dot::DotOperation::execute(){
  operationCallback(*this);
}
