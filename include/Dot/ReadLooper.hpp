#ifndef _READ_LOOPER
#define _READ_LOOPER

#include <vector>
#include <Dot/Reader.hpp>
class ReadLooper;
#include <Dot/Dot.hpp>

extern "C" {
  #include <comm.h>
}

namespace dot {
  /*
   * Reads the socket indefinitely
   * Provides register interface, where Readers can register
   * The register (Reader)s will be called when a new message arrives
   */
class ReadLooper{
private:
  static ReadLooper *instance;
  bool shouldRun = false;
  std::vector<Reader *> registeredReaders;
    ReadLooper();
public:
  comm_socket _sock;
  static ReadLooper &getReadLooper(comm_socket);
  static ReadLooper &getReadLooper();
  void run(Dot &);
  void stop(Dot &);
  void registerReader(Reader &);
};
}

#endif
