#include <iostream>
#include <Dot/Dot.hpp>

int main(int argc, char *argv[])
{
    using namespace dot;
    Dot server;
    server.setReadCallback([](Dot d, std::string msg) {
        d.writeLine("OK");
    });
    server.listen(8080); //blocking call
    return 0;
}