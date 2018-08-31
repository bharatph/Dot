#ifndef _READER_H
#define _READER_H
#include <iostream>
#include <Dot/DotOperation.hpp>
#include <Dot/Dot.hpp>

namespace dot {
class Reader : public DotOperation{
    private:
    Dot *dot;
    public:
    Reader(Dot *dot);
    Reader &read(std::string message);
};
}
#endif
