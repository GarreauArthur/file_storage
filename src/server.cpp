#include "App.h"
#include <fstream>
#include "multipart-parser/MultipartParser.h"

static void
onPartBegin(std::string_view buffer, size_t start, size_t end, void *userData) {
  printf("onPartBegin\n");
}

static void
onHeaderField(std::string_view buffer, size_t start, size_t end, void *userData) {
  std::cout << "BREAKPOINT onHeaderField" << std::endl;
  if ( buffer == NULL ) std::cout << "Buffer is NULL" << std::endl;
  else std::cout << buffer.substr(start, end-start) << std::endl;
  //printf("onHeaderField: (%s)\n", 0, end-start, buffer);
}

static void
onHeaderValue(std::string_view buffer, size_t start, size_t end, void *userData) {
  printf("onHeaderValue\n");
  if ( buffer == NULL ) std::cout << "Buffer is NULL" << std::endl;
  else std::cout << buffer.substr(start, end-start) << std::endl;
}

static void
onPartData(std::string_view buffer, size_t start, size_t end, void *userData) {
  printf("onPartData\n");
}

static void
onPartEnd(std::string_view buffer, size_t start, size_t end, void *userData) {
  printf("onPartEnd\n");
}

static void
onEnd(std::string_view buffer, size_t start, size_t end, void *userData) {
  printf("onEnd\n");
}

int main( void ) {


  uWS::App().get("/*", [](auto *res, auto *req) {
    res->end("Hello world!");
  })
  
  .post("/upload", [](auto *res, auto *req) {

    for ( auto it = req->begin(); it != req->end(); ++it)
      std::cout << "key: " << std::get<0>(*it) << " Value: " << std::get<1>(*it) << std::endl;
    std::string buffer;

    std::ofstream myfile("file.binary");

    //todo move upper
    MultipartParser parser;
    parser.onPartBegin = onPartBegin;
    parser.onHeaderField = onHeaderField;
    parser.onHeaderValue = onHeaderValue;
    parser.onPartData = onPartData;
    parser.onPartEnd = onPartEnd;
    parser.onEnd = onEnd;
    std::string boundary = std::string{req->getHeader("content-type")}.substr(30);
    std::cout << boundary << std::endl << std::endl;
    parser.setBoundary(boundary);
    bool isSet = false;

    res->onData([res, buffer = std::move(buffer), myfile = std::move(myfile), parser = std::move(parser), isSet = std::move(isSet)](std::string_view data, bool last) mutable {

      std::cout << parser.boundary << "onData" << std::endl;
      if( data.length() == 0 ) return;
      buffer.append(data.data(), data.length());

      myfile << data;
      /* 
      if ( !isSet )
      {
        std::string boundary;
        for (int i = 0; i < buffer.size(); i++) {
          boundary += buffer[i];
          if ( buffer[i] == '\n' ) {
            break;
          }
        }
        std::cout << boundary << std::endl;
        parser.setBoundary(boundary);
        isSet = true;
      }*/
      std::cout << parser.feed(data, data.length()) << std::endl;

      if ( last ) {
        if ( parser.hasError() )
          std::cout << parser.getErrorMessage() << std::endl;
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

