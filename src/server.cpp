#include "App.h"
#include <fstream>

int main( void ) {


  uWS::App().get("/*", [](auto *res, auto *req) {
    res->end("Hello world!");
  })
  
  .post("/upload", [](auto *res, auto *req) {

    std::string buffer;

    std::ofstream myfile("file.binary");

    res->onData([res, buffer = std::move(buffer), myfile = std::move(myfile)](std::string_view data, bool last) mutable {

      buffer.append(data.data(), data.length());
      myfile << data;

      if ( last ) {
        res->end("Thanks for the data!");
      }
    });

    res->onAborted([res](){
      std::cerr << "response aborted" << std::endl;
      res->end("error, aborted");
    });
  
  }) 
  
  
  
  
  .listen(3000, [](auto *token) {
    if (token) {
      std::cout << "Listening on port " << 3000 << std::endl;
    }
  }).run();

  std::cout << "Failed to listen on port 3000" << std::endl;

  return 0;

}

