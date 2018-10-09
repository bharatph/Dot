#include <Dot/Reader.hpp>
#include <Dot/DotLooper.hpp>
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
  dot->getLooper().registerReader(*this, message);
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
