#ifndef STATUS_HANDLER_HPP
#define STATUS_HANDLER_HPP

#include <string>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_config.hpp"
#include "log.hpp"


/// The common handler for all incoming requests.
class StatusHandler
  : public RequestHandler
{
public:
  StatusHandler();
  Status Init(const std::string& uri_prefix,
                      const NginxConfig& config);
  Status HandleRequest(const Request& request,
                               Response* response);
};

REGISTER_REQUEST_HANDLER(StatusHandler);
#endif // STATUS_HANDLER_HPP
