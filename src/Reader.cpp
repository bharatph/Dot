#include <Dot/Reader.hpp>

dot::Reader::Reader()
{
}

dot::Reader::Reader(Dot *dot)
{
    this->dot = dot;
}

dot::Reader &dot::Reader::read(std::string message)
{
    //get socket from dot
    //if(error){
    //fireEvent(DotOperationEvent::FAILED, *dot, *this);
    //} else {
    //fireEvent(DotOperationEvent::SUCCESS, *dot, *this);
    //}
    return *this;
}
