#ifndef _DOT
#define _DOT "Dot"

#include <iostream>
#include <thread>
#include <functional>
#include <future>
#include <vector>
#include <map>
#include <queue>

#include <Dot/Reader.hpp>
#include <Dot/Writer.hpp>
#include <Dot/DotOperation.hpp>
#include <Dot/DotEventManager.hpp>

extern "C" {
#include <comm.h>
}

namespace dot {
class Dot : public DotEventManager {
    private:
    comm_socket _sock;
    static Dot *instance;
    std::map<std::string, EventCallback> readForMap;
    std::queue<DotOperation> incomingQueue;
    std::queue<DotOperation> outgoingQueue;
    std::future<void> runner;
    void _init();
    void _readLoop();
    Dot();
    Dot(const Dot &dot);
    protected:
    public:
    static Dot &getDot();
    void connect(std::string host, int port);
    void disconnect();
    void resume();
    comm_socket getSocket();
    Writer &write(std::string message);
    Reader &readFor(int binaryFile, std::string fileType);
    Reader &readFor(std::string message);
    void run();
    ~Dot();
};
}
#endif
