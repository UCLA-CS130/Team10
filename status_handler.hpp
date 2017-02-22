#ifndef STATUS_HANDLER_HPP
#define STATUS_HANDLER_HPP

#include <string>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_config.hpp"


/// The common handler for all incoming requests.
class StatusHandler
  : public RequestHandler
{
public:
  StatusHandler();
  Status Init(const std::string& uri_prefix,
                      const ServerConfig& config);
  Status HandleRequest(const Request& request,
                               Response* response);

private:
  std::string m_uri_prefix;
  std::string m_path;
};


#endif // STATUS_HANDLER_HPP
