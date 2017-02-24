//
// status_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "status_handler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

StatusHandler::StatusHandler()
{

}

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  return RequestHandler::OK;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request,
                               Response* response)
{
  std::cout << "StatusHandler HandleRequest...\n";
  std::string result = Log::instance()->map_to_string();
  result += '\n';
  result += Log::instance()->records_to_string();
  std::cout << result << '\n';
  response->SetStatus(Response::ok);
  response->AddHeader("Content-Length", std::to_string(result.size()));
  response->AddHeader("Content-Type", "text/plain");
  response->SetBody(result);
  return RequestHandler::OK;
}
