//
// status_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "status_handler.hpp"
#include <fstream>
#include <sstream>

StatusHandler::StatusHandler()
{

}

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
                      const ServerConfig& config)
{
  m_uri_prefix = uri_prefix;
  m_path = config.Status();
  return RequestHandler::OK;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request,
                               Response* response)
{
  // TODO:
  return RequestHandler::OK;
}
