#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;
Dot *server;

void serve(Dot d, std::string msg)
{
    if (msg == "close")
    {
        d.writeLine("Closing server");
        server->close();
        return;
    }
    std::cout << msg << std::endl;
}

int main(int argc, char *argv[])
{
    server = new Dot();
    server->setReadCallback(serve);
    server->listen(8080); //blocking call
    return 0;
}