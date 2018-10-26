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
          dot.readFor(msgs).onSuccess([](Dot &dot, std::string message) {
            std::cout << "good day read" << std::endl;
            dot.write("good day");
          });
        })
      .onDisconnect([](Dot &dot) {
        std::cout << "server closed" << std::endl;
      });
  return server;
}

Dot &modular_server()
{
  Dot &server = Dot::getDot(3500);
  server
      .onConnect([](Dot &dot) {
        std::cout << "Client: " << dot.getUid() << " connected" << std::endl;
        dot.readFor("([a-z])*")
            .onSuccess([](Dot &dot, std::string message) {
              std::cout << dot.getUid() << ": " << message << std::endl;
            })
            .onFailed([](Dot &dot, std::string message) {
              std::cout << "Unmatched syntax" << std::endl;
            });
        dot.onDisconnect([](Dot &dot) {
          std::cout << "Client: " << dot.getUid() << " disconnected" << std::endl;
        });
      })
      .onDisconnect([](Dot &dot) {
        std::cout << "Server closed successfully" << std::endl;
      })
      .onFailed([](Dot &dot) {
        std::cout << "Error occured, check the logs" << std::endl;
      });
  return server;
}

Dot &read_all_server()
{
  Dot &server = Dot::getDot(3500);
  server.onConnect([](Dot &dot) {
    dot.readFor("([a-z])*").onSuccess([](Dot &dot, std::string message) {
                             std::cout << message << std::endl;
                           })
        .onFailed([](Dot &dot, std::string message) {
          std::cout << "error message" << std::endl;
        });
    dot.onDisconnect([](Dot &dot) {
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
            .onSuccess([](Dot &dot, std::string message) {
              std::cout << "hello read" << std::endl;
              dot.sendFile("./Dot_server_test");
            });
      })
      .onDisconnect([](Dot &) {
        std::cout << "disconnected" << std::endl;
      });
  return server;
}

//TODO FIXME server and regex test
int main(int argc, char *argv[])
{
  Dot &server = modular_server();
  char c = 'a';
  std::cin >> c;
  if (c == 'q')
  {
    server.disconnect();
    return 0;
  }
  while (1)
    ;
  return 0;
}
