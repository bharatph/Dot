#include <iostream>
#include <Dot/Dot.hpp>
#include <Dot/DotOperationEvent.hpp>

using namespace dot;

Dot &setup_and_start_server(){
      Dot &server = Dot::getDot();
      server.on(DotEvent::CONNECTED, [](Dot &dot){
  	  dot.readFor("hi").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message) {
  		  std::cout << "hi read" << '\n';
  	  });
  	  dot.readFor("hello").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message) {
  		  std::cout << "hello read" << '\n';
  		  dot.write("hello back");
  	  });
      std::vector<std::string> msgs;
      msgs.push_back("good");
      msgs.push_back("day");
      dot.readFor(msgs).on(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message){
        std::cout << "good day read" << std::endl;
        dot.write("good day");
      });
      }).on(DotEvent::DISCONNECTED, [](Dot &dot){
        std::cout << "server closed" << std::endl;
      });
      return server;
}

int main(int argc, char *argv[])
{
    Dot &server = setup_and_start_server();
    char c = 'a';
    std::cin >> c;
    if(c == 'q'){
      server.disconnect();
      return 0;
    }
    return server.run();
}
