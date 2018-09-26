#ifndef _READER_H
#define _READER_H
#include <iostream>
#include <vector>
#include <Dot/DotOperation.hpp>

namespace dot
{
  class Dot;
  /*
   * Reader acts as a "Handler" for ReadLooper
   */
class Reader : public DotOperation
{
  private:
    Dot *dot;
    std::string _message;

  public:
    Reader();
    Reader(Dot *dot);
      dot::Reader &read();
      dot::Reader &readFor(int binaryFile, std::string fileType);
      dot::Reader &readFor(std::string message);
      dot::Reader &readFor(std::vector<std::string>);
    std::string getMessage();
    void notify(std::string) override; //TODO add timestamp with message
};
}
#endif
