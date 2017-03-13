#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"
#include "encoder.hpp"
#include "server_config.hpp"


/// The common handler for all incoming requests.
class StaticHandler
  : public RequestHandler
{
public:
  StaticHandler();
  Status Init(const std::string& uri_prefix,
                      const NginxConfig& config);
  Status HandleRequest(const Request& request,
                               Response* response);


private:
  std::string m_uri_prefix;
  std::string m_static_path;
  bool m_compression_enabled;
  bool url_decode(const std::string& in, std::string& out);
};


REGISTER_REQUEST_HANDLER(StaticHandler);
#endif // FILE_HANDLER_HPP
