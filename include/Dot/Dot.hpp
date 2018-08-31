#ifndef _DOT
#define _DOT "Dot"

#include <iostream>
#include <thread>
#include <functional>
#include <future>
#include <vector>
#include <map>
#include <queue>

#include <Dot/DotEventManager.hpp>
#include <Dot/DotOperation.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Writer.hpp>

extern "C" {
#include <comm/comm.h>
}

namespace dot {
class Dot : public DotEventManager {
    private:
    typedef int comm_socket; //TODO REMOVE
    comm_socket _sock;
    std::map<std::string, EventCallback> readForMap;
    std::queue<DotOperation> incomingQueue;
    std::queue<DotOperation> outgoingQueue;
    comm_socket getSocket();
    std::future<void> runner;
    void _init();
    void _readLoop();
    protected:
    public:
    Dot();
    Dot(const Dot &dot);
    void connect(std::string host, int port);
    void disconnect();
    void resume();
    Writer &write(std::string message);
    Reader &readFor(int binaryFile, std::string fileType);
    Reader &readFor(std::string message);
    void run();
    ~Dot();
};
}
#endif
