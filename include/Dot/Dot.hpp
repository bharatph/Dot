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
#include <Dot/DotEventManager.hpp>
#include <Dot/DotOperation.hpp>
#include <crossguid/guid.hpp>

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

class Dot : public DotEventManager
{
private:
  comm_socket current_sock;
  xg::Guid uid;
  std::vector<Dot *> connectedDots;
  static std::map<int, Dot *> *instances;
  //DotLooper *readLooper;
  std::queue<DotOperation *> incomingQueue;
  std::queue<DotOperation *> outgoingQueue;
  std::future<void> runner;
  std::thread *serverThread = nullptr;
  bool shouldRun = false;
  bool readText = false;
  std::map<DotOperation *, std::string> textReaders;
  std::thread *runnerThread = nullptr;
  bool shouldServerRun = false;

  Dot();
  Dot &listen(int port);

protected:
public:
  Dot(comm_socket);
  //returns the dot of the current system
  static Dot &getDot(int port);
  static Dot &getDot();
  Dot &connect(std::string host, int port);
  Dot &disconnect();
  void resume();
  comm_socket getSocket();
  DotOperation &write(std::string message);
  DotOperation &read();
  DotOperation &readFor(int binaryFile, std::string fileType);
  std::string getUid();
  /*
     * Regex supported readFor
     */
  DotOperation &readFor(std::string message);
  DotOperation &readFor(std::vector<std::string> messages);

  void run();
  void stop();
  int sendFile(std::string);
  std::ofstream &readFile(int toRead);
  void registerReader(DotOperation &, std::string);
  ~Dot();
};
} // namespace dot
#endif
