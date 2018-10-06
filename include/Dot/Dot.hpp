#ifndef _DOT
#define _DOT "Dot"
#if defined(_MSC_VER)
#if _MSC_VER < 1800
#error This project needs atleast Visual Studio 2013
#endif
#elif __cplusplus <= 199711L
#error This project can only be compiled with a compiler that supports C++11
#endif

#include <iostream>
#include <thread>
#include <functional>
#include <future>
#include <vector>
#include <map>
#include <queue>

#include <Dot/DotEvent.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Writer.hpp>
#include <Dot/ReadLooper.hpp>

extern "C"
{
#include <comm.h>
#include <clog/clog.h>
}

namespace dot
{
/*
  	 * All connected device is a Dot
  	 * The current host is also a Dot
     * Every dot has read and write operation
     * Read is either blocking or non-blocking
     * Write is non-blocking
  	 */
class Dot : protected em::EventManager<dot::DotEvent, Dot &>
{
private:
  comm_socket current_sock;
  std::vector<Dot *> connectedDots;
  static std::map<int, Dot *> *instances;
  ReadLooper *readLooper;
  std::map<std::string, EventCallback> readForMap;
  std::queue<DotOperation> incomingQueue;
  std::queue<DotOperation> outgoingQueue;
  std::future<void> runner;
  std::thread *serverThread = nullptr;
  bool shouldServerRun = false;
  Dot &listen(int port);
  Dot();
  Dot(const Dot &dot);

protected:
public:
  Dot(comm_socket);
  Dot &on(DotEvent, EventCallback);
  //returns the dot of the current system
  static Dot &getDot(int port);
  static Dot &getDot();
  Dot &connect(std::string host, int port);
  Dot &disconnect();
  void resume();
  comm_socket getSocket();
  ReadLooper &getReadLooper();
  Writer &write(std::string message);
  Reader &read();
  Reader &readFor(int binaryFile, std::string fileType);
  /*
     * Regex supported readFor
     */
  Reader &readFor(std::string message);
  Reader &readFor(std::vector<std::string> messages);
  int run();
  ~Dot();
};
} // namespace dot
#endif
