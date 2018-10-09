#include <iostream>
#include <Dot/Dot.hpp>
#include <Dot/DotOperationEvent.hpp>
#include <string>

using namespace dot;

Dot &setup_and_start_server()
{
  Dot &server = Dot::getDot(3500);
  server.on(DotEvent::CONNECTED, [](Dot &dot) {
          dot.readFor("hi").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message) {
            std::cout << "hi read" << '\n';
          });
          dot.readFor("hello").addEventHandler(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message) {
            std::cout << "hello read" << '\n';
          });
          std::vector<std::string> msgs;
          msgs.push_back("good");
          msgs.push_back("day");
          dot.readFor(msgs).on(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message) {
            std::cout << "good day read" << std::endl;
            dot.write("good day");
          });
        })
      .on(DotEvent::DISCONNECTED, [](Dot &dot) {
        std::cout << "server closed" << std::endl;
      });
  return server;
}

Dot &read_all_server()
{

  Dot &server = Dot::getDot(3500);
  server.on(DotEvent::CONNECTED, [](Dot &dot) {
    dot.readFor("([a-z])*").on(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message) {
                             std::cout << message << std::endl;
                           })
        .on(DotOperationEvent::FAILED, [](Dot &dot, std::string message) {
          std::cout << "error message" << std::endl;
        });
    dot.on(DotEvent::DISCONNECTED, [](Dot &dot) {
      std::cout << "Client disconnected" << std::endl;
    });
  });
  return server;
}

Dot &send_binary()
{
  Dot &server = Dot::getDot(3500);
  server
      .on(DotEvent::CONNECTED, [](Dot &dot) {
        dot.readFor("hello")
            .on(DotOperationEvent::SUCCESS, [](Dot &dot, std::string message) {
              std::cout << "hello read" << std::endl;
              dot.getLooper().sendFile("./Dot_server_test");
            });
      })
      .on(DotEvent::DISCONNECTED, [](Dot &) {
        std::cout << "disconnected" << std::endl;
      });
  return server;
}

int main(int argc, char *argv[])
{
  Dot &server = send_binary();
  char c = 'a';
  std::cin >> c;
  if (c == 'q')
  {
    server.disconnect();
    return 0;
  }
  return server.run();
}
