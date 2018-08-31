#include <Dot/Reader.cpp>

dot::Reader::Reader(){
   this->dot = dot;
}

dot::Reader &dot::Reader::read(std::string message){
  //get socket from dot
    //if(error){
        //fireEvent(DotOperationEvent::FAILED, *dot, *this);
    //} else {
        //fireEvent(DotOperationEvent::SUCCESS, *dot, *this);
    //}
    return *this;
}
