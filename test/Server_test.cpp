#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;

int main(int argc, char *argv[])
{
    Dot &server = Dot::getDot();
    server.readFor("hi").addDotOperationEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot){
      std::cout << "hi read" << '\n';
    });
    server.readFor("hello").addDotOperationEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot){
      std::cout << "hello read" << '\n';
      dot.write("hello back");
    });
    return server.run();
}
