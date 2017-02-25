//
// echo_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//


#ifndef ECHO_HANDLER_HPP
#define ECHO_HANDLER_HPP

#include <string>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"


/// The common handler for all incoming requests.
class EchoHandler
  : public RequestHandler
{
public:
  EchoHandler();
  Status Init(const std::string& uri_prefix,
                      const NginxConfig& config);
  Status HandleRequest(const Request& request,
                               Response* response);
};

REGISTER_REQUEST_HANDLER(EchoHandler);
#endif // ECHO_HANDLER_HPP
