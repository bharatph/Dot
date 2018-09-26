#ifndef _DOT
#define _DOT "Dot"
#if defined(_MSC_VER)
#   if _MSC_VER < 1800
#       error This project needs atleast Visual Studio 2013
#   endif
#elif __cplusplus <= 199711L
#   error This project can only be compiled with a compiler that supports C++11
#endif


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
#include <clog/clog.h>
}

namespace dot {
	/*
	 * All connected device is Dot
	 * The current host is also a Dot
	 */
class Dot : protected em::EventManager<dot::DotEvent, Dot &> {
    private:
    comm_socket current_sock;
    std::vector<Dot *> connectedDots;
    static Dot *instance;
    Reader *reader;
    Writer *writer;
    ReadLooper *readLooper;
    std::queue<DotOperation> incomingQueue;
    std::queue<DotOperation> outgoingQueue;
    std::future<void> runner;
    std::thread *serverThread = nullptr;
	bool shouldServerRun = false;
    void _init();
    void _readLoop();
    Dot();
    Dot(const Dot &dot);

    protected:

    public:
    Dot(comm_socket);
    Dot &on(DotEvent, EventCallback);
    //returns the dot of the current system
    static Dot &getDot();
    Dot &connect(std::string host, int port);
    Dot &disconnect();
    void resume();
	comm_socket getSocket();
    dot::ReadLooper &getReadLooper();
    Reader &getReader();
    Writer &getWriter();
    int run();
    ~Dot();
};
}
#endif
