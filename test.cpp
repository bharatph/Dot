#include <iostream>
#include <comm/comm.h>

enum class DotState{
  CONNECT,
  DISCONNECT,
  PAUSE,
  RESUME
};

class DotStateHelper {
public:
  DotStateHelper(){
  }

  std::string valueOf(DotState dotState){
    switch(dotState){
      case DotState::CONNECT:
        return "CONNECT";
      case DotState::DISCONNECT:
        return "DISCONNECT";
      case DotState::PAUSE:
        return "PAUSE";
      case DotState::RESUME:
        return "RESUME";
      default:
        return "";
    }
  }
};

class DotStateMetadata{
public:
  DotStateMetadata(){
  }
};

class Dot {
public:
  Dot(){
  }
  Dot(std::string host, int port){
    connect(host, port);
  }
  void connect(std::string host, int port){
    int i = comm_connect_server(host.c_str(), port);
    if(i > 0){
      onSuccess(DotState(), DotStateMetadata());
    } else {
      onError(DotState(), DotStateMetadata());
    }
  }
  void disconnect(){
  }
  //callbacks
  virtual void onSuccess(DotState dotState, DotStateMetadata dotStateMetadata){  
  }
  virtual void onError(DotState dotState, DotStateMetadata dotStateMetadata){
  }
};

class LightDot : public Dot {
  public:
    LightDot(){
    }
    void onSuccess(DotState dotState, DotStateMetadata dotStateMetadata) override {
      //handle states using if
    }
};

int main(int argc, char *argv[]){
  //SOLO basic pgm
  //TODO load object from db
  LightDot lightDot;
  lightDot.connect("localhost", 22);
}
