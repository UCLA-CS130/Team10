//
// file_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "file_handler.hpp"
#include <fstream>
#include <sstream>

FileHandler::FileHandler()
{

}

RequestHandler::Status FileHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  m_uri_prefix = uri_prefix;
  return RequestHandler::OK;
}

RequestHandler::Status FileHandler::HandleRequest(const Request& request,
                               Response* response)
{
  response->SetStatus(Response::ok);
  response->AddHeader("Content-Length", std::to_string(request.raw_request().size()));
  response->AddHeader("Content-Type", "text/plain");
  response->SetBody(request.raw_request());
  return RequestHandler::OK;
}
