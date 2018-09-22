#ifndef _DOT
#define _DOT "Dot"

#include <iostream>
#include <thread>
#include <functional>
#include <future>
#include <vector>
#include <map>
#include <queue>

#include <Dot/DotEvent.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Writer.hpp>
#include <Dot/ReadLooper.hpp>

extern "C" {
#include <comm.h>
#include <clog.h>
}

namespace dot {

class Dot : public em::EventManager<DotEvent, Dot &> {
    private:
    comm_socket current_sock;
    std::vector<Dot *> connectedDots;
    static Dot *instance;
    ReadLooper *readLooper;
    std::map<std::string, EventCallback> readForMap;
    std::queue<DotOperation> incomingQueue;
    std::queue<DotOperation> outgoingQueue;
    std::future<void> runner;
    std::thread *serverThread = nullptr;
    void _init();
    void _readLoop();
    Dot();
    Dot(const Dot &dot);

    protected:

    public:
    Dot(comm_socket);
    //returns the dot of the current system
    static Dot &getDot();
    Dot &connect(std::string host, int port);
    Dot &disconnect();
    void resume();
    comm_socket getSocket();
    ReadLooper &getReadLooper();
    Writer &write(std::string message);
    Reader &read();
    Reader &readFor(int binaryFile, std::string fileType);
    Reader &readFor(std::string message);
    int run();
    ~Dot();
};
}
#endif
