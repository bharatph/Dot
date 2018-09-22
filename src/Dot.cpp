#include <Dot/Dot.hpp>
#include <Dot/ReadLooper.hpp>
#include <future>

    static const char *TAG = "Dot";

    dot::Dot *dot::Dot::instance = nullptr;

    comm_socket dot::Dot::getSocket(){
        return _sock;
    }

    void dot::Dot::_init(){
        //initialize socket structure
    }
    void dot::Dot::_readLoop(){
        //while(1){
        //TODO std::string received = comm_read(sock);
        //TODO if received == the following iteration call that function
            std::map<std::string, EventCallback>::iterator it;
            for(it = readForMap.begin(); it != readForMap.end(); ++it){
                DotOperation *dotOperation = new DotOperation();
                it->second(*this);
            }
        //}
    }


    dot::Dot::Dot(){
      readLooper = &ReadLooper::getReadLooper();
      //readLooper->run();//TODO non-blocking
      serverThread = new std::thread ([&](){
        //run with defualt port
          _sock = comm_start_server(3500);
          if(_sock < 1){
            fireEvent(DotEvent::DISCONNECTED, *this);
            return;
          }
          fireEvent(DotEvent::CONNECTED, *this);
          readLooper->_sock = _sock;
          readLooper->run(*this);
      });
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
        _sock = comm_connect_server(host.c_str(), port);
        fireEvent(DotEvent::CONNECTED, *this);
        _readLoop();
        return (*this);
    }
    dot::Dot &dot::Dot::disconnect(){
        //TODO if connected disconnect, else don't bother
        //DotOperation dotOperation;
        //dotState.val = "disconnected";
        if( comm_close_socket(_sock) < 0){
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

    int dot::Dot::read(void *buffer, size_t length){
      return ::recv(_sock, buffer, length, 0);
    }

    int dot::Dot::write(std::string buffer, size_t length){
      char *temp = (char *)calloc(sizeof(char), length);
      strcpy(temp, buffer.c_str());
      return ::send(_sock, temp, length, 0);
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
