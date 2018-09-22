#include <Dot/Reader.hpp>
#include <Dot/ReadLooper.hpp>
#include <Dot/Dot.hpp>

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
  dot->getReadLooper().registerReader(*this);
  return *this;
}

void dot::Reader::notify(std::string message){
      fireEvent(DotOperationEvent::SUCCESS, *dot);
}

std::string dot::Reader::getMessage(){
  return _message;
}
