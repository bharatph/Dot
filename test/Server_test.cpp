#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;

int main(int argc, char *argv[])
{
    Dot &server = *(new Dot());
    server.connect("localhost", 22);
    return 0;
}
