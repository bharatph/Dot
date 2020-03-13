Dot {#mainpage}
=========
Asynchronous network library for C++ targeted for TCP/IPv4 stack with no protocol overhead

### Why no protocol overhead?
Because Dot does not have protocol. Read and writes are performed for binary and text which means protcols can be implemented in Dot. For modularity reasons Dot does not perform encyption or decryption.

### Why the dot terminology?
A Dot can be said to be the smallest breakable part in a picture. When imagining a network with several different dots(devices), operations on dot become intuitive

A general usage of Dot would require
```cpp
#include <Dot/Dot.hpp>
#include <Dot/DotOperationEvent.hpp>
using namespace dot;
```

```cpp
Dot &dot = Dot::getDot();
dot.on(DotOperationEvent::CONNECTED, [](Dot &dot){
  dot.write("hi node");
});
dot.connect("localhost", 3500);
```

Peer to peer Chat application using dot
```cpp
using namespace std;
Dot &dot = Dot::getDot();
dot.on(DotOperationEvent::CONNECTED, [](Dot &dot, string message){
  dot.readFor("*").on(DotEventOperation::SUCCESS, [](Dot &dot){ //read any string
    string message;
    cout << message << endl;//hook with ui
  });
  std::string message = " ";
  while(message != "\q"){
    cin >> message;
    dot.write(message).on(DotOperationEvent::SUCCESS, [](Dot &dot, string message){

    });
  }
});
dot.connect("localhost", 3500);
return dot.run(); //runs unless dot is explicitly asked to disconnect
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
