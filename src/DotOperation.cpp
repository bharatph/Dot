#include <Dot/DotOperation.hpp>

dot::DotOperation::DotOperation(){

}

void dot::DotOperation::notify(std::string){
  //no-op, meant to be overwritten
}

dot::DotOperation::~DotOperation(){
  std::cout << "Ending" << std::endl;
}

dot::DotOperation &dot::DotOperation::on(dot::DotOperationEvent dotOperationEvent, EventCallback eventCallback){
  addEventHandler(dotOperationEvent, eventCallback);
  return *this;
}
