#include <Dot/Reader.hpp>
#include <Dot/ReadLooper.hpp>
#include <Dot/Dot.hpp>

dot::Reader::Reader()
{
}

dot::Reader::Reader(const Reader &reader)
{
  this->dot = reader.dot;
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

void dot::Reader::notify(std::string message)
{
  if (message == "")
  {
    fireEvent(DotOperationEvent::FAILED, *dot, message);
    return;
  }
  fireEvent(DotOperationEvent::SUCCESS, *dot, message);
}

std::string dot::Reader::getMessage()
{
  return _message;
}
