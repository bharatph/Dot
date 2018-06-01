# Dot
Asynchronous network library for C++ targeted for TCP/IPv4 stack with no protocol overhead

### Why the dot terminology?
A Dot can be said to be the smallest breakable part in a picture. When imagining a network with several different dots(devices), operations on dot become intuitive

### Why no protocol overhead?
Because Dot does not have protocol. Read and writes are performed for binary and text which means protcols can be implemented in Dot. For modularity reasons Dot does not perform encyption or decryption.


A general usage of Dot would require
```cpp
#include <Dot/Dot.hpp>
using namespace dot;
```

### Server side usage
```cpp
Dot server;
server.response("Hi", [](Dot d) {
    d.write("Hello");
});
server.listen(8080); //blocking call
```

### Client side usage
```cpp
Dot client;
client.connect("localhost", 8080);
client.write("Hi");
client.close();
```

### Installation
Dot can be installed with CMake on any POSIX like system with
```sh
mkdir build
cd build/
cmake ..
make
make install
```

### Minimum Requirements
Dot relies on C++11 lambda functions
Compiles on gcc, clang, mingw and cygwin