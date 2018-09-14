#include <iostream>
#include <Dot/Dot.hpp>

int main(int argc, char *argv[])
{
    using namespace dot;
    Dot &client = Dot::getDot();
    client.write("Hi");
    client.connect("localhost", 8080);
    return 0;
}
