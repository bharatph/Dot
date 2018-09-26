#include <Dot/Dot.hpp>
#include <Dot/ReadLooper.hpp>
#include <Dot/Reader.hpp>

dot::Reader::Reader() {}

dot::Reader::Reader(Dot *dot) { this->dot = dot; }

dot::Reader &dot::Reader::read(std::string message) {
  this->_message = message;
  dot->getReadLooper().registerReader(*this);
  return *this;
}

dot::Reader &dot::Reader::read(std::vector<std::string> messages) {
  this->_message = messages.at(0);
  dot->getReadLooper().registerReader(*this);
  return *this;
}

dot::Reader &dot::Reader::read() {
  // TODO: insert return statement here
  Reader &reader = (*new Reader());
  return reader;
}

dot::Reader &dot::Reader::readFor(int binaryFile, std::string fileType) {
  Reader &reader = *(new Reader(this));
  return reader.read(fileType);
}

dot::Reader &dot::Reader::readFor(std::string message) {
  Reader &reader = *(new Reader(this));
  return reader.read(message);
}

dot::Reader &dot::Reader::readFor(std::vector<std::string> messages) {
  Reader &reader = *(new Reader(this));
  return reader.read(messages);
}

void dot::Reader::notify(std::string message) {
  fireEvent(DotOperationEvent::SUCCESS, *dot);
}

std::string dot::Reader::getMessage() { return _message; }
