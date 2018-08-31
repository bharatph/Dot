#ifndef _WRITER_H
#define _WRITER_H

#include <Dot/DotOperation.hpp>
#include <Dot/Dot.hpp>

namespace dot {
//sends message, after callback is completed destroys itself
  class Writer : public DotOperation {
  private:
    Dot *dot;
  public:
    Writer(Dot *dot);
    DotOperation &write(std::string message);
  };
}
#endif
