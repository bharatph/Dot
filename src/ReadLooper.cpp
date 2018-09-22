#include <Dot/ReadLooper.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Dot.hpp>

extern "C" {
  #include <clog.h>
  #include <comm.h>
}

static const char *TAG = "ReadLooper";


dot::ReadLooper::ReadLooper(){

}
dot::ReadLooper::ReadLooper(Dot *dot) : dot(dot){

}


//use libev insted to read
void dot::ReadLooper::run(){
  shouldRun = true;
while(shouldRun){
  //read the main socket in regular intervals
  char *buffer = (char *)calloc(sizeof(char), 1024);
  int stat = recv(dot->getSocket(), buffer, 20, 0);
  if(stat < 0){
    log_err(TAG, "Error reading");
    shouldRun = false;
  } else if (stat == 0){
    shouldRun = false;
  }
  //read line
  int line_no = 0;
  if(buffer == NULL)continue;
  char **lines = read_line(&line_no, buffer);
  if(lines == NULL)continue;
  if(line_no < 1)continue;
  //compare read line with registered readers
  for(Reader *reader : registeredReaders){
    if(strstr(lines[0], reader->getMessage().c_str()) != NULL){
      reader->notify(buffer);//FIXEME send timestamp instead of string
    }
  }
}
}

void dot::ReadLooper::stop(){
  shouldRun = false;

}

void dot::ReadLooper::registerReader(Reader &reader){
  registeredReaders.push_back(&reader);
}
