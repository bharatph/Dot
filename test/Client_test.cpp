#include <iostream>
#include <fstream>
#include <Dot/Dot.hpp>

using namespace dot;
int main(int argc, char *argv[])
{
  Dot &dot = Dot::getDot(0);
  dot.connect("localhost", 3500)
      .onConnect([](Dot &dot) {
        dot.write("hello");
        //std::cout<< "reading file" << std::endl;
        std::ofstream &file = dot.readFile(200);
        file.close();
        dot.on(DotEvent::DISCONNECTED, [](Dot &dot) {
          std::cout << "disconnected" << '\n';
        });
        //dot.disconnect();
      })
      .onDisconnect([](Dot &dot) {
        std::cout << "disconnected" << '\n';
      });
      while(1);
      return 0;
}
