#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;

void onConnected(Dot &dot){
    dot.write("hello");
    dot.write("hi");
    dot.on(DotEvent::DISCONNECTED, [](Dot &dot){
      std::cout << "disconnected" << '\n';
    });
    dot.disconnect();
}

int main(int argc, char *argv[])
{
    Dot &dot = Dot::getDot();
    dot.connect("localhost", 3500)
    .on(DotEvent::CONNECTED, onConnected)
    .on(DotEvent::TIMEOUT, [](Dot &dot){
    })
    .on(DotEvent::DISCONNECTED, [](Dot &dot){
      std::cout << "disconnected" << '\n';
    });
    return dot.run();
}
