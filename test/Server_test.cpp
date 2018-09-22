#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;

int main(int argc, char *argv[])
{
    Dot &server = Dot::getDot();
    server.readFor("hi").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot){
      std::cout << "hi read" << '\n';
    });
    server.readFor("hello").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot){
      std::cout << "hello read" << '\n';
      dot.write("hello back");
    });
    server.addEventHandler(DotEvent::CONNECTED, [](Dot &dot){
      std::cout << "message" << '\n';
    });
    return server.run();
}
