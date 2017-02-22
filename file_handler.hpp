#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"


/// The common handler for all incoming requests.
class FileHandler
  : public RequestHandler
{
public:
  FileHandler();
  Status Init(const std::string& uri_prefix,
                      const NginxConfig& config);
  Status HandleRequest(const Request& request,
                               Response* response);

private:
  std::string m_uri_prefix;
  std::string m_static_path;
  //TODO: alter to hash map for all static paths
};


#endif // FILE_HANDLER_HPP
