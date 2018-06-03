#ifndef _DOT
#define _DOT "Dot"

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <functional>

#include <comm/comm.h>

namespace dot
{
enum class WriteMode
{
  WAIT_UNTIL_CONNECTION,
  NO_WAIT
};

class Dot
{
private:
  bool closeServerFlag = false;
  std::string hostname;
  int port;
  std::function<void(Dot, std::string)> readCallback;
  std::vector<Dot> activeConnections;
  SOCKET sock;
  Dot(SOCKET sock);
  void serve(Dot dot);

public:
  Dot();
  Dot(const Dot &dot);
  ~Dot();
  //binary read
  void *read();
  //text read
  std::string readLine();
  std::string *readLinePtr();
  std::string getHostname();
  void setReadCallback(std::function<void(Dot, std::string)>);
  int getPortNumber();
  std::vector<std::thread *> serverThreads;
  void write(std::string msg, WriteMode writeMode = WriteMode::WAIT_UNTIL_CONNECTION);
  void writeLine(std::string msg, WriteMode writeMode = WriteMode::WAIT_UNTIL_CONNECTION);
  void response(std::string requestMessage, std::function<void(Dot)> responseFunction);
  void listen(int port);

  void connect(std::string hostname, int port);
  void connect(Dot dot);
  void connect(std::vector<Dot> dots);
  void close();
};
}
#endif