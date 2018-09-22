#include <Dot/Dot.hpp>

#include <Dot/DotOperation.hpp>
#include <Dot/ReadLooper.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Writer.hpp>

#include <future>

    static const char *TAG = "Dot";

    dot::Dot *dot::Dot::instance = nullptr;


    comm_socket dot::Dot::getSocket(){
        return current_sock;
    }

    dot::Dot::Dot(){
      //readLooper->run();//TODO non-blocking
      serverThread = new std::thread ([&](){
        //run with defualt port
        while(1){
          comm_socket sock = comm_start_server(3500);
          if(sock < 1){
            fireEvent(DotEvent::DISCONNECTED, *this);
            return;
          }
          Dot *dot = new Dot(sock);
          connectedDots.push_back(dot);
          fireEvent(DotEvent::CONNECTED, *dot);
        }
      });
    }

    dot::Dot::Dot(comm_socket sock){
      this->current_sock = sock;
      readLooper = new ReadLooper(this);
    }

    dot::ReadLooper &dot::Dot::getReadLooper(){
      return *readLooper;
    }


    dot::Dot::Dot(const Dot &dot){

    }

    dot::Dot &dot::Dot::getDot(){
      if(instance == nullptr){
        instance = new Dot();
      }
      return *instance;
    }

    dot::Dot &dot::Dot::connect(std::string host, int port){
        //DotOperation dotOperation;
        //dotState.val = "connected";
        //int connection = comm_connnect(sockfd);
        comm_socket sock = comm_connect_server(host.c_str(), port);
        if(sock < 1){
          log_err(_DOT, "Connection failed");
          fireEvent(DotEvent::DISCONNECTED, *this);
        }
        Dot *dot = new Dot(sock);
        connectedDots.push_back(dot);
        fireEvent(DotEvent::CONNECTED, *dot);
        return (*this);
    }
    dot::Dot &dot::Dot::disconnect(){
        //TODO if connected disconnect, else don't bother
        //DotOperation dotOperation;
        //dotState.val = "disconnected";
        if( comm_close_socket(current_sock) < 0){
          //print error
          fireEvent(DotEvent::ERROR, *this);
          return *this;
      }
      fireEvent(DotEvent::DISCONNECTED, *this);
      return *this;
    }
    void dot::Dot::resume(){
        //DotOperation dotOperation;
        //dotState.val = "resume";
        fireEvent(DotEvent::RESUME, *this);
    }
    dot::Writer &dot::Dot::write(std::string message){
        //add to queue and let runner decide whether to run or not
        //DotState dotState;
        //writeFunc(dotState);
        Writer &writer = *(new Writer(this));
        outgoingQueue.push(writer.write(message));
        return writer;
    }

    dot::Reader &dot::Dot::readFor(int binaryFile, std::string fileType){
        Reader &reader = *(new Reader(this));
        //readForMap[fileType] = readForCallback;
        return reader.read(fileType);
    }

    dot::Reader &dot::Dot::readFor(std::string message){
        Reader &reader = *(new Reader(this));
        //readForMap[message] = readForCallback;
        return reader.read(message);
    }

    /*
    Reader &readFor(std::vector<std::string> messages){
        Reader *reader = new Reader();
        for(std::string message : messages){
            reads[message] = readFunc;
            reader.read(messages)
        }
        return *reader;
    }
    */

    int dot::Dot::run(){
        //runner = std::async(std::launch::async, &Dot::_run, this);
        //_readLoop();
        while(1); //wait till ever
        return 0;
    }
    dot::Dot::~Dot(){
        std::cout << "Dot ended" << std::endl;
        serverThread->join();
    }
