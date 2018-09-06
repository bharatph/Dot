#ifndef _ONCONNECTED_H
#define _ONCONNECTED_H

#include <iostream>
#include <Dot/Dot.hpp>

class OnConnect {
    public:
    virtual void onConnect(std::string host, int port, Dot &dot);
};

#endif