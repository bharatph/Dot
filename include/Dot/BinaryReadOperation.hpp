#ifndef _DOT_BIN_READ_OP
#define _DOT_BIN_READ_OP

#include <Dot/Operation.hpp>

namespace dot {
  class Dot;
  class BinaryReadOperation : public dot::Operation {
  public:
    BinaryReadOperation(Dot &, int);
  };
}

#endif
