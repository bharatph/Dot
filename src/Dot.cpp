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
        Dot *d = new Dot(sock);
        activeConnections.push_back(d);
        //serverThreads.push_back();
    }
}

void dot::Dot::setReadCallback(std::function<void(Dot, std::string)> callback)
{
    readCallback = callback;
}

template <class T>
void dot::Dot::setReadCallback(T *t, void (T::*func)(Dot, std::string))
{
    readCallback = std::bind(func, t);
}

std::string *dot::Dot::readLinePtr()
{
    const char *msg = comm_read_text(this->sock);
    if (msg == NULL)
    {
        return nullptr;
    }
    return new std::string(msg);
}

void dot::Dot::serve()
{
    std::string *msg;
    while ((msg = this->readLinePtr()) != nullptr)
    {
        readCallback(*this, *msg);
    }
}

dot::Dot::Dot(SOCKET sock)
{
    this->sock = sock;
    std::thread t = std::thread(&Dot::serve, this);
}

void dot::Dot::close()
{
    closeServerFlag = true;
    for (Dot *dot : activeConnections)
    {
        dot->close();
    }
    activeConnections.clear();
    std::terminate();
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
    activeConnections[sock] = new Dot(sock);
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