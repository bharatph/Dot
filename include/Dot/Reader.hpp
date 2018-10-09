#ifndef _READER_H
#define _READER_H
#include <iostream>
#include <vector>
#include <Dot/DotOperation.hpp>

namespace dot
{
  class Dot;
  /*
   * Reader acts as a "Handler" for DotLooper
   */
class Reader : public DotOperation
{
  private:
    Dot *dot;

  public:
    Reader();
    Reader(Dot *dot);
    Reader(const Reader &);
    Reader &read(std::string message);
    void notify(std::string) override; //TODO add timestamp with message
};
}
#endif
