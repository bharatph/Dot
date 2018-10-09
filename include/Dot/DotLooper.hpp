#ifndef _READ_LOOPER
#define _READ_LOOPER

#include <vector>
#include <thread>
#include <fstream>

extern "C"
{
#include <comm.h>
}

#include <Dot/Reader.hpp>
#include <Dot/DotEvent.hpp>

#include <em/EventManager.hpp>

namespace dot
{
class Dot;
/*
   * Reads the socket indefinitely
   * Provides register interface, where Readers can register
   * The register (Reader)s will be called when a new message arrives
   */
class DotLooper : public em::EventManager<DotEvent>
{
private:
  Dot *dot = nullptr;
  bool shouldRun = false;
  bool readText = false;
  std::vector<Reader *> registeredReaders;
  std::thread *runnerThread = nullptr;

protected:
  DotLooper();

public:
  DotLooper(Dot *dot);
  void run();
  void stop();
  int sendFile(std::string);
  std::ofstream &readFile(int toRead);
  void registerReader(Reader &);
  ~DotLooper();
};
} // namespace dot

#endif
