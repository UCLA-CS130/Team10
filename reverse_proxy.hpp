#ifndef REVERSE_PROXY_HPP
#define REVERSE_PROXY_HPP

#include <string>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_config.hpp"


/// The common handler for all incoming requests.
class ReverseProxy
  : public RequestHandler
{
public:
  ReverseProxy();
  Status Init(const std::string& uri_prefix,
                      const NginxConfig& config);
  Status HandleRequest(const Request& request,
                               Response* response);


private:
  std::string remote_host;
  std::string remote_port;
  std::string m_uri_prefix;
};


REGISTER_REQUEST_HANDLER(ReverseProxy);
#endif // REVERSE_PROXY_HPP
