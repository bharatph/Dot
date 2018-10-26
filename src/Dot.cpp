#include <Dot/Dot.hpp>

#include <iostream>
#include <future>
#include <fstream>

#include <Dot/DotEvent.hpp>
#include <Dot/DotOperation.hpp>
#include <regex>

extern "C"
{
#include <comm.h>
}

std::map<int, dot::Dot *> *dot::Dot::instances = nullptr;

static const char *TAG = "Dot";

comm_socket dot::Dot::getSocket()
{
  return current_sock;
}

dot::Dot::Dot()
{
  uid = xg::newGuid();
  run();
}

dot::Dot::Dot(comm_socket sock) : Dot()
{
  this->current_sock = sock;
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
        fireEvent(DotEvent::FAILED, *this);
        return;
      }
      std::cout << sock << std::endl;
      Dot *dot = new Dot(sock);
      connectedDots.push_back(dot);
      fireEvent(DotEvent::CONNECTED, *dot);
    }
  },
                                 port);
  return *this;
}

dot::Dot &dot::Dot::getDot(int port)
{
  if (instances == nullptr)
  {
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

std::string dot::Dot::getUid()
{
  return uid.str();
}

static int i = -1; //negative ports indicate client dots
dot::Dot &dot::Dot::getDot()
{
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
dot::DotOperation &dot::Dot::write(std::string message)
{
  //add to queue and let runner decide whether to run or not
  //DotState dotState;
  //writeFunc(dotState);
  DotOperation *operation = new DotOperation(*this, [&](DotOperation &dotOperation) {
    char *temp = strdup(message.c_str());
    //comm_write_text(dot->getSocket(), message.c_str(), message.en);u
    size_t wrote = ::send(current_sock, temp, message.length(), 0); //TODO use libcomm to support atomic functions
    if (wrote < 1)
    {
      log_err(TAG, "Opeation failed");
      dotOperation.fireEvent(DotOperationEvent::FAILED, *this, message);
    }
    else
    {
      dotOperation.fireEvent(DotOperationEvent::SUCCESS, *this, message);
    }
  });
  operation->execute();//TODO delegate to the looper
  outgoingQueue.push(operation);
  return *operation;
}

dot::DotOperation &dot::Dot::read()
{
  // TODO: insert return statement here
  DotOperation &reader = (*new DotOperation(*this, [](DotOperation &dotOperation){

  }));
  return reader;
  //TODO read and call events
  // DotOperation &operation = new DotOperation(*this, [](DotOperation &dotOperation){
  //   ::recv(current_sock, buf, )
  // });
}

dot::DotOperation &dot::Dot::readFor(int binaryFile, std::string fileType)
{
  DotOperation &reader = *(new DotOperation(*this, [](DotOperation &){

  }));
  //return reader.read(fileType);
  return reader;
}

dot::DotOperation &dot::Dot::readFor(std::string message)
{
  DotOperation &reader = *(new DotOperation(*this, [](DotOperation &operationCallback){

  }));
  registerReader(reader, message);
  return reader;
}

dot::DotOperation &dot::Dot::readFor(std::vector<std::string> messages)
{
  if (messages.size() == 0)
  {
    //throw exception perhaps?
    return *(new DotOperation(*this, [](DotOperation &operationCallback){

    }));
  }

  // for (std::string message : messages)
  // {
  //   reader
  // }
  //FIXME
  return *(new DotOperation(*this, [](DotOperation &operationCallback){

  }));
}

//use libev insted to read
void dot::Dot::run()
{
  runnerThread = new std::thread([&]() {
    shouldRun = true;
    while (shouldRun)
    {
      if (textReaders.size() == 0)
      {
        continue;
      }
      //read the main socket in regular intervals
      char *buffer = comm_read_text(current_sock, 1024);
      if (buffer == NULL)
      {
        log_inf(TAG, "Client disconnected");
        shouldRun = false;
        fireEvent(DotEvent::DISCONNECTED, *this);
        continue;
      }
      //compare read line with registered readers
      for (std::pair<DotOperation *, std::string> textReaderPair : textReaders)
      {
        std::regex reg(textReaderPair.second);
        textReaderPair.first->fireEvent(std::regex_match(buffer, reg) ? DotOperationEvent::SUCCESS : DotOperationEvent::FAILED, textReaderPair.first->getDot(), buffer); //TODO send timestamp with messages
      }
    }
  });
}

void dot::Dot::stop()
{
  shouldRun = false;
}

int dot::Dot::sendFile(std::string fileName)
{
  std::ifstream file;
  file.open(fileName);
  if (file.fail())
  {
    return -1;
  }
  char *buffer = (char *)malloc(200);
  file.read(buffer, 200);
  if (buffer == NULL)
  {
    return -1;
  }
  int fileSize = strlen(buffer);
  //this->stop();
  std::cout << "file sent" << std::endl;
  return ::send(current_sock, buffer, fileSize, 0);
}

std::ofstream &dot::Dot::readFile(int toRead)
{
  std::ofstream &file = *(new std::ofstream());
  file.open("./dotFile");
  char *buffer = (char *)malloc(toRead);
  std::cout << "file reading from client" << std::endl;
  //this->stop();
  ::recv(current_sock, buffer, toRead, 0); //FIXME
  file.write(buffer, toRead);
  return file;
}

void dot::Dot::registerReader(DotOperation &reader, std::string message)
{
  textReaders[&reader] = message;
}

dot::Dot::~Dot()
{
  runnerThread->join();
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
