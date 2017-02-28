#ifndef REVERSE_PROXY_HPP
#define REVERSE_PROXY_HPP

#include <string>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_config.hpp"


/// The common handler for all incoming requests.
class ReverseProxyHandler
  : public RequestHandler
{
public:
  ReverseProxyHandler();
  Status Init(const std::string& uri_prefix,
                      const NginxConfig& config);
  void handle(const boost::system::error_code& ec,
        std::size_t bytes_transferred);
  Status HandleRequest(const Request& request,
                               Response* response);
  RequestHandler::Status SendProxyRequest(const std::string& request_string, const std::string& new_host, Response* response,
                                                      const int& attempt_num = 0);
  std::pair<std::string, std::string> ProcessHeaderLine(std::string header);

private:

  std::string m_remote_host;
  std::string m_remote_port;
  std::string m_uri_prefix;
};


REGISTER_REQUEST_HANDLER(ReverseProxyHandler);
#endif // REVERSE_PROXY_HPP
