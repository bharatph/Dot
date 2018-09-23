#include <iostream>
#include <Dot/Dot.hpp>

using namespace dot;

int main(int argc, char *argv[])
{
    Dot &server = Dot::getDot();
    server.addEventHandler(DotEvent::CONNECTED, [](Dot &dot){
	  dot.readFor("hi").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot) {
		  std::cout << "hi read" << '\n';
	  });
	  dot.readFor("hello").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot) {
		  std::cout << "hello read" << '\n';
		  dot.write("hello back");
	  });
    });
    return server.run();
}
