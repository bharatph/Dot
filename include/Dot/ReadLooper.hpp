#ifndef _READ_LOOPER
#define _READ_LOOPER

#include <vector>
#include <thread>

extern "C"
{
#include <comm.h>
}

#include <Dot/Reader.hpp>
#include <Dot/ReadLooperEvent.hpp>

#include <em/EventManager.hpp>

namespace dot
{
class Dot;
/*
   * Reads the socket indefinitely
   * Provides register interface, where Readers can register
   * The register (Reader)s will be called when a new message arrives
   */
class ReadLooper : public em::EventManager<ReadLooperEvent>
{
private:
  Dot *dot = nullptr;
  bool shouldRun = false;
  std::vector<Reader *> registeredReaders;
  std::thread *runnerThread = nullptr;

protected:
  ReadLooper();

public:
  ReadLooper(Dot *dot);
  void run();
  void stop();
  void registerReader(Reader &);
  ~ReadLooper();
};
} // namespace dot

#endif
