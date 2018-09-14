#ifndef _READER_H
#define _READER_H
#include <iostream>
#include <Dot/DotOperation.hpp>

namespace dot
{
  class DotOperation;
class Reader : public DotOperation
{
  private:
    Dot *dot;

  public:
    Reader();
    Reader(Dot *dot);
    Reader &read(std::string message);
};
}
#endif
