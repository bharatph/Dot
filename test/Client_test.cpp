#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;
int main(int argc, char *argv[])
{
  Dot &dot = Dot::getDot(0);
  dot.connect("localhost", 3500)
      .on(DotEvent::CONNECTED, [](Dot &dot) {
        dot.write("hello");
        //std::cout<< "reading file" << std::endl;
        std::ofstream &file = dot.getLooper().readFile(200);
        file.close();
        dot.on(DotEvent::DISCONNECTED, [](Dot &dot) {
          std::cout << "disconnected" << '\n';
        });
        //dot.disconnect();
      })
      .on(DotEvent::TIMEOUT, [](Dot &dot) {
      })
      .on(DotEvent::DISCONNECTED, [](Dot &dot) {
        std::cout << "disconnected" << '\n';
      });
  return dot.run();
}
