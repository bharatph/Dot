#include <Dot/Dot.hpp>

dot::Dot::Dot()
{
}

void dot::Dot::response(std::string requestMessage, std::function<void(Dot)> responseFunc)
{
}

void dot::Dot::listen(int port)
{
}

void dot::Dot::connect(std::string hostname, int port)
{
}

void *dot::Dot::read()
{
}

std::string dot::Dot::readLine()
{
    return "";
}
void dot::Dot::write(std::string msg, WriteMode writeMode)
{
    switch (writeMode)
    {
    case WriteMode::WAIT_UNTIL_CONNECTION:
        break;
    case WriteMode::NO_WAIT:
        break;
    }
}

void dot::Dot::connect(dot::Dot dot)
{
}

void dot::Dot::connect(std::vector<dot::Dot> dots)
{
}