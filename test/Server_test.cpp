#include <iostream>
#include <Dot/Dot.hpp>

int main(int argc, char *argv[])
{
    using namespace dot;
    Dot server;
    server.response("Hi", [](Dot d) {
        d.write("hello");
    });
    server.listen(8080); //blocking call
    return 0;
}