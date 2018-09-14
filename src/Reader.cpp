#include <Dot/Reader.hpp>
#include <Dot/ReadLooper.hpp>

dot::Reader::Reader()
{
}

dot::Reader::Reader(Dot *dot)
{
    this->dot = dot;
}

dot::Reader &dot::Reader::read(std::string message)
{
  this->_message = message;
  ReadLooper::getReadLooper().registerReader(*this);
  //register read event with read looper
    //get socket from dot
    //if(error){
    //fireEvent(DotOperationEvent::FAILED, *dot, *this);
    //} else {
    //fireEvent(DotOperationEvent::SUCCESS, *dot, *this);
    //}
    //const char *buffer = (char *)calloc(sizeof(char), message.length());
    //comm_read_text(dot->getSocket(), buffer, message.length());
    return *this;
}

void dot::Reader::notify(std::string message){
  if(message == _message){
    fireEvent(DotOperationEvent::SUCCESS, *dot);
  }
}
