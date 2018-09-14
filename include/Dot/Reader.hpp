#ifndef _READER_H
#define _READER_H
#include <iostream>
#include <Dot/DotOperation.hpp>

namespace dot
{
  class DotOperation;
  /*
   * Reader acts as a "Handler" for ReadLooper
   */
class Reader : public DotOperation
{
  private:
    Dot *dot;
    std::string _message;

  public:
    Reader();
    Reader(Dot *dot);
    Reader &read(std::string message);
    void notify(std::string) override; //TODO add timestamp with message
};
}
#endif
