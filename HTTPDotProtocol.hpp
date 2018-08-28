
class HTTPDotProtocol : public DotProtocol, public em::EventManager<HTTPProtocolState, const DotState &> {
public:
  //without ssl
  class HTTPRequest{
      public:
      HTTPRequest(){

      }
  };
  class HTTPResponse{
      public:
      HTTPResponse(){

      }
  };
  class HTTPRequestBuilder {
      public:
      HTTPRequestBuilder(){

      }
      HTTPRequestBuilder &setHeader(std::string header){
          return *this;
      }
      HTTPRequestBuilder &setBody(std::string body){
          return *this;
      }
      HTTPRequest &build(){
          return *(new HTTPRequest());
      }

  };
  HTTPDotProtocol(){
      run();
  }
  //with ssl
  HTTPDotProtocol(std::string secret){

  }

  void start() override {
     running = true;
    std::cout << "Starting HTTP" << std::endl;
  }
  void end() override {
      std::cout << "Closing HTTP" << std::endl;
  }
  void send(std::string message) override {
      //comm_write(sockfd, "send");
      HTTPRequestBuilder hrb;
      HTTPRequest hr = hrb.setHeader("POST upload/some_file")
      .setBody(message).build();
      std::cout << "sent: " << message << std::endl;
  }
  void recv() override {

  }
  ~HTTPDotProtocol(){
      kill();
  }
};
