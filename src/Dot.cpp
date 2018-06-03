#include <Dot/Dot.hpp>

dot::Dot::Dot()
{
}

dot::Dot::Dot(const dot::Dot &dot)
{
}
dot::Dot::~Dot()
{
    this->close();
}

void dot::Dot::response(std::string requestMessage, std::function<void(Dot)> responseFunc)
{
}

void dot::Dot::listen(int port)
{
    while (closeServerFlag == false)
    {
        SOCKET sock = comm_start_server(port);
        Dot d = Dot(sock);
        serverThreads.push_back(new std::thread(&Dot::serve, this, d));
    }
}

void dot::Dot::setReadCallback(std::function<void(Dot, std::string)> callback)
{
    readCallback = callback;
}

std::string *dot::Dot::readLinePtr()
{
    return new std::string(comm_read_text(this->sock));
}

void dot::Dot::serve(Dot dot)
{
    std::string *msg;
    while ((msg = this->readLinePtr()) != nullptr)
    {
        readCallback(dot, *msg);
    }
}

dot::Dot::Dot(SOCKET sock)
{
    this->sock = sock;
}

void dot::Dot::close()
{
    closeServerFlag = true;
    activeConnections.clear();
}

void *dot::Dot::read()
{
}

std::string dot::Dot::readLine()
{
    std::string *msg = this->readLinePtr();
    if (msg == nullptr)
    {
        return std::string();
    }
    return *msg;
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

void dot::Dot::writeLine(std::string msg, WriteMode writeMode)
{
    comm_write_text(this->sock, msg.c_str());
    switch (writeMode)
    {
    case WriteMode::WAIT_UNTIL_CONNECTION:
        break;
    case WriteMode::NO_WAIT:
        break;
    }
}

std::string dot::Dot::getHostname()
{
    return hostname;
}

int dot::Dot::getPortNumber()
{
    return port;
}

void dot::Dot::connect(std::string hostname, int port)
{
    SOCKET sock = comm_connect_server(hostname.c_str(), port);
    activeConnections[sock] = Dot(sock);
}

void dot::Dot::connect(dot::Dot dot)
{
    connect(dot.getHostname(), dot.getPortNumber());
}

void dot::Dot::connect(std::vector<dot::Dot> dots)
{
    for (const Dot &dot : dots)
    {
        this->connect(dot);
    }
}