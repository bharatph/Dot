#include <Dot/Dot.hpp>

#include <iostream>
#include <future>

#include <Dot/DotEvent.hpp>
#include <Dot/DotOperation.hpp>
#include <Dot/ReadLooperEvent.hpp>
#include <Dot/ReadLooper.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Writer.hpp>

extern "C"
{
#include <comm.h>
}

std::map<int ,dot::Dot *> *dot::Dot::instances = nullptr;

static const char *TAG = "Dot";

comm_socket dot::Dot::getSocket()
{
  return current_sock;
}

dot::Dot::Dot()
{
}

dot::Dot::Dot(comm_socket sock)
{
  this->current_sock = sock;
  readLooper = new ReadLooper(this);
  readLooper->addEventHandler(ReadLooperEvent::DISCONNECTED, [&](){
    fireEvent(DotEvent::DISCONNECTED, *this);
  });
  readLooper->run();
}

dot::Dot &dot::Dot::listen(int port)
{
  //readLooper->run();//TODO non-blocking
  shouldServerRun = true;
  serverThread = new std::thread([&](int port_val) {
    //run with defualt port
    while (shouldServerRun)
    {
      comm_socket sock = comm_start_server(port_val);
      if (sock < 1)
      {
        fireEvent(DotEvent::DISCONNECTED, *this);
        return;
      }
      std::cout << sock << std::endl;
      Dot *dot = new Dot(sock);
      connectedDots.push_back(dot);
      fireEvent(DotEvent::CONNECTED, *dot);
    }
  }, port);
  return *this;
}

dot::ReadLooper &dot::Dot::getReadLooper()
{
  if (readLooper == nullptr)
  {
    log_err(_DOT, "Cannot use ReadLooper with local node");
    throw readLooper;
  }
  return *readLooper;
}

dot::Dot::Dot(const Dot &dot)
{
}

dot::Dot &dot::Dot::getDot(int port)
{
  if(instances == nullptr){
    instances = new std::map<int, Dot *>();
  }
  if ((*instances)[port] == nullptr)
  {
    if (comm_init() < 0)
    {
      log_err(_DOT, "Cannot initialize Dot, make sure you have all the prerequisites installed");
    }
    (*instances)[port] = new Dot();
    (*instances)[port]->listen(port);
  }
  return *(*instances)[port];
}

static int i = -1;
dot::Dot &dot::Dot::getDot(){
  return getDot(i--);
}

dot::Dot &dot::Dot::connect(std::string host, int port)
{
  std::thread *connectionThread = new std::thread([&](int port_param) {
    comm_socket sock = comm_connect_server(host.c_str(), port_param);
    if (sock < 1)
    {
      log_err(_DOT, "Connection failed");
      fireEvent(DotEvent::DISCONNECTED, *this);
      return;
    }
    Dot *dot = new Dot(sock);
    connectedDots.push_back(dot);
    fireEvent(DotEvent::CONNECTED, *dot);
  },
                                                  port);
  //connectionThread->detach();
  return (*this);
}

dot::Dot &dot::Dot::on(DotEvent dotEvent, EventCallback eventCallback)
{
  addEventHandler(dotEvent, eventCallback);
  return (*this);
}

dot::Dot &dot::Dot::disconnect()
{
  //TODO if connected disconnect, else don't bother
  //DotOperation dotOperation;
  //dotState.val = "disconnected";
  if (comm_close_socket(current_sock) < 0)
  {
    //print error
    log_err(_DOT, "Cannot close socket properly");
  }
  shouldServerRun = false;
  fireEvent(DotEvent::DISCONNECTED, *this);
  return *this;
}
void dot::Dot::resume()
{
  //DotOperation dotOperation;
  //dotState.val = "resume";
  fireEvent(DotEvent::RESUME, *this);
}
dot::Writer &dot::Dot::write(std::string message)
{
  //add to queue and let runner decide whether to run or not
  //DotState dotState;
  //writeFunc(dotState);
  Writer &writer = *(new Writer(this));
  outgoingQueue.push(writer.write(message));
  return writer;
}

dot::Reader &dot::Dot::read()
{
  // TODO: insert return statement here
  Reader &reader = (*new Reader());
  return reader;
}

dot::Reader &dot::Dot::readFor(int binaryFile, std::string fileType)
{
  Reader &reader = *(new Reader(this));
  return reader.read(fileType);
}

dot::Reader &dot::Dot::readFor(std::string message)
{
  Reader &reader = *(new Reader(this));
  return reader.read(message);
}

dot::Reader &dot::Dot::readFor(std::vector<std::string> messages)
{
  if (messages.size() == 0)
  {
    //throw exception perhaps?
    return *(new Reader(this));
  }

  Reader *reader = new Reader(this);
  for (std::string message : messages)
  {
    reader->read(message);
  }
  return *reader;
}

int dot::Dot::run()
{
  //runner = std::async(std::launch::async, &Dot::_run, this);
  //_readLoop();
  while (shouldServerRun)
    ; //wait till ever
  return 0;
}
dot::Dot::~Dot()
{
  //stop accepting further connections
  shouldServerRun = false;
  //close any existing connections
  for (Dot *dot : connectedDots)
  {
    dot->disconnect();
  }
  //wait for the thread to end
  serverThread->join();
}
