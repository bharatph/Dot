#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;

int main(int argc, char *argv[])
{
    Dot &server = Dot::getDot();
    server.connect("localhost", 3500);
    server.write("hello").addDotOperationEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot){
      std::cout << "written" << '\n';
    });
    server.readFor("hi").addDotOperationEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot){
      std::cout << "read message" << '\n';
    });
    while(1);
    return 0;
}
