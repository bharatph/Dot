#include <Dot/Dot.hpp>

#include <Dot/DotEvent.hpp>
#include <Dot/DotOperation.hpp>
#include <Dot/ReadLooper.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Writer.hpp>

extern "C" {
#include <comm.h>
}

#include <future>

    static const char *TAG = "Dot";

    dot::Dot *dot::Dot::instance = nullptr;


    comm_socket dot::Dot::getSocket(){
        return current_sock;
    }

    dot::Dot::Dot(){
      reader = new Reader(this);
      writer = new Writer(this);
      //readLooper->run();//TODO non-blocking
		shouldServerRun = true;
      serverThread = new std::thread ([&](){
        //run with defualt port
        while(shouldServerRun){
			comm_socket sock = comm_start_server(3500);
          if(sock < 1){
            fireEvent(DotEvent::DISCONNECTED, *this);
            return;
          }
		  std::cout << sock << std::endl;
          Dot *dot = new Dot(sock);
          connectedDots.push_back(dot);
          fireEvent(DotEvent::CONNECTED, *dot);
        }
      });
    }

    dot::Dot::Dot(comm_socket sock){
      this->current_sock = sock;
      readLooper = new ReadLooper(this);
	  readLooper->run();
    }

    dot::ReadLooper &dot::Dot::getReadLooper(){
		if (readLooper == nullptr) {
			log_err(_DOT, "Cannot use ReadLooper with local node");
			throw readLooper;
		}
      return *readLooper;
    }


    dot::Dot::Dot(const Dot &dot){

    }

    dot::Dot &dot::Dot::getDot(){
      if(instance == nullptr){
		  //if error occurs, log the error message and further operations on Dot will be no-op
		  if (comm_init() < 0) {
			  log_fat(_DOT, "Cannot initialize Dot, make sure you have all the prerequisites installed");
		  }
		  instance = new Dot();
      }
      return *instance;
    }

    dot::Dot &dot::Dot::connect(std::string host, int port){
        std::thread *connectionThread = new std::thread([&](int port_param){
        comm_socket sock = comm_connect_server(host.c_str(), port_param);
        if(sock < 1){
          log_err(_DOT, "Connection failed");
          fireEvent(DotEvent::DISCONNECTED, *this);
          return;
        }
        Dot *dot = new Dot(sock);
        connectedDots.push_back(dot);
        fireEvent(DotEvent::CONNECTED, *dot);
      }, port);
      //connectionThread->detach();
        return (*this);
    }

    dot::Dot &dot::Dot::on(DotEvent dotEvent, EventCallback eventCallback){
      addEventHandler(dotEvent, eventCallback);
      return (*this);
    }

    dot::Dot &dot::Dot::disconnect(){
        //TODO if connected disconnect, else don't bother
        //DotOperation dotOperation;
        //dotState.val = "disconnected";
        if( comm_close_socket(current_sock) < 0){
          //print error
          fireEvent(DotEvent::DISCONNECTED, *this);
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

    dot::Reader &dot::Dot::getReader(){
      return *reader;
    }

    dot::Writer &dot::Dot::getWriter(){
      return *writer;
    }

    int dot::Dot::run(){
        //runner = std::async(std::launch::async, &Dot::_run, this);
        //_readLoop();
        while(1); //wait till ever
        return 0;
    }
    dot::Dot::~Dot(){
		//stop accepting further connections
		shouldServerRun = false;
		//close any existing connections
		for (Dot* dot : connectedDots) {
			dot->disconnect();
		}
		//wait for the thread to end
        serverThread->join();
    }
