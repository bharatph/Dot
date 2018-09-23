#ifndef _WRITER_H
#define _WRITER_H

#include <Dot/DotOperation.hpp>

namespace dot {
	class Dot;
//sends message, after callback is completed destroys itself
class Writer : public DotOperation
{
private:
  Dot *dot;
  std::string _message;

public:
  Writer();
  Writer(Dot *dot);
  Writer &write(std::string message);
};
}
#endif
