#ifndef _DOT
#define _DOT "Dot"

#include <iostream>
#include <vector>
#include <functional>

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
  //binary read
  void *read();
  //text read
  std::string readLine();

public:
  Dot();
  void write(std::string msg, WriteMode writeMode = WriteMode::WAIT_UNTIL_CONNECTION);
  void response(std::string requestMessage, std::function<void(Dot)> responseFunction);
  void listen(int port);

  void connect(std::string hostname, int port);
  void connect(Dot);
  void connect(std::vector<Dot> dots);
};
}
#endif