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

dot::DotOperation &dot::DotOperation::on(dot::DotOperationEvent dotOperationEvent, EventCallback eventCallback)
{
  addEventHandler(dotOperationEvent, eventCallback);
  return *this;
}

dot::DotOperation &dot::DotOperation::onSuccess(EventCallback eventCallback)
{
  return on(DotOperationEvent::SUCCESS, eventCallback);
}

dot::DotOperation &dot::DotOperation::onFailed(EventCallback eventCallback)
{
  return on(DotOperationEvent::FAILED, eventCallback);
}

dot::DotOperation &dot::DotOperation::onTimeout(EventCallback eventCallback)
{
  return on(DotOperationEvent::TIMEOUT, eventCallback);
}

void dot::DotOperation::execute(){
  operationCallback(*this);
}
