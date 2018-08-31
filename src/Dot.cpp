#include <Dot/Dot.hpp>
    dot::Dot::comm_socket dot::Dot::getSocket(){
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

    }
    dot::Dot::Dot(const Dot &dot){

    }
    void dot::Dot::connect(std::string host, int port){
        //DotOperation dotOperation;
        //dotState.val = "connected";
        //int connection = comm_connnect(sockfd);
        fireEvent(DotEvent::CONNECTED, *this);
        _readLoop();
    }
    void dot::Dot::disconnect(){
        //TODO if connected disconnect, else don't bother
        //DotOperation dotOperation;
        //dotState.val = "disconnected";
        fireEvent(DotEvent::DISCONNECTED, *this);
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

    void dot::Dot::run(){
        //runner = std::async(std::launch::async, &Dot::_run, this);
        _readLoop();
    }
    dot::Dot::~Dot(){
        std::cout << "Dot ended" << std::endl;
    }
