#include <Dot/ReadLooper.hpp>

extern "C" {
  #include <clog.h>
}

static const char *TAG = "ReadLooper";

dot::ReadLooper *dot::ReadLooper::instance = nullptr;
comm_socket dot::ReadLooper::_sock = -1;

dot::ReadLooper::ReadLooper(){

}

dot::ReadLooper &dot::ReadLooper::getReadLooper(){
  if(instance == nullptr){
    instance = new ReadLooper();
  }
  return *instance;
}

dot::ReadLooper &dot::ReadLooper::getReadLooper(comm_socket sock){
_sock = sock;
return getReadLooper();
}

void dot::ReadLooper::run(){
  shouldRun = true;
while(shouldRun){
  //read the main socket in regular intervals
  char *buffer = (char *)calloc(sizeof(char), 1024);
  int stat = recv(_sock, buffer, 20, 0);
  if(stat < 0){
    log_err(TAG, "Error reading");
    shouldRun = false;
  } else if (stat == 0){
    shouldRun = false;
  }
  for(Reader *reader : registeredReaders){
    reader->notify(buffer);
  }
  //TODO read until a \r\n or \r or \n hits
    //reader.notify(buffer);
}
}

void dot::ReadLooper::stop(){
  shouldRun = false;
}

void dot::ReadLooper::registerReader(Reader &reader){
  registeredReaders.push_back(&reader);
}
