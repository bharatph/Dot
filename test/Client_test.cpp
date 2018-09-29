#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;
int main(int argc, char *argv[])
{
    Dot &dot = Dot::getDot();
    dot.connect("localhost", 3500)
    .on(DotEvent::CONNECTED, [](Dot &dot){
      dot.readFor("hello back").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message) {
        dot.write("hi");
      });
    dot.write("hello");
    dot.write("hi");
    dot.on(DotEvent::DISCONNECTED, [](Dot &dot){
      std::cout << "disconnected" << '\n';
    });
    dot.disconnect();
    })
    .on(DotEvent::TIMEOUT, [](Dot &dot){
    })
    .on(DotEvent::DISCONNECTED, [](Dot &dot){
      std::cout << "disconnected" << '\n';
    });
    return dot.run();
}
