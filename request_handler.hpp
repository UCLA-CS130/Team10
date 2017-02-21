//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//


#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <string>


// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class request_handler
{
public:
  enum Status {
    OK = 0;
    // Define your status codes here.
  };
  
  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config) = 0;

  // Handles an HTTP request, and generates a response. Returns a response code
  // indicating success or failure condition. If ResponseCode is not OK, the
  // contents of the response object are undefined, and the server will return
  // HTTP code 500.
  virtual Status HandleRequest(const Request& request,
                               Response* response) = 0;
};


#endif // HTTP_REQUEST_HANDLER_HPP
