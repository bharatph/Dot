#include <Dot/Writer.hpp>
#include <Dot/Dot.hpp>
#include <comm.h>
#include <clog.h>

static const char *TAG = "Writer";

dot::Writer::Writer()
{
}

dot::Writer::Writer(Dot *dot)
{
    this->dot = dot;
}

dot::Writer &dot::Writer::write(std::string message)
{
    //comm_write_text(dot->getSocket(), message.c_str(), message.en);
    log_inf(TAG, "writing");
    ::write(dot->getSocket(), message.c_str(), message.length());
    //int bytes_written = comm_write(sockfd, message);
    //if(bytes_written < 0){
    //fireEvent(DotOperationEvent::FAILED, *dot, *this);
    //} else {
    //fireEvent(DotOperationEvent::SUCCESS, *dot, *this );
    //}
    return *this;
}


void dot::Writer::notify(std::string message){
  if(message == _message){
    fireEvent(DotOperationEvent::SUCCESS, *dot);
  }
}
