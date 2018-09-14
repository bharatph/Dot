#ifndef _READ_LOOPER
#define _READ_LOOPER

#include <vector>
#include <Dot/Reader.hpp>

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
  static comm_socket _sock;
  static ReadLooper *instance;
  bool shouldRun = false;
  std::vector<Reader *> registeredReaders;
    ReadLooper();
public:
  static ReadLooper &getReadLooper(comm_socket);
  static ReadLooper &getReadLooper();
  void run();
  void stop();
  void registerReader(Reader &);
};
}

#endif
