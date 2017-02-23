//
// not_found_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "not_found_handler.hpp"


NotFoundHandler::NotFoundHandler()
{

}

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  return RequestHandler::OK;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request,
                               Response* response)
{
  // TODO: Make the line below work
  // response = reply::stock_reply(reply::not_found);
  response->SetStatus(Response::not_found);
  response->AddHeader("Content-Length", std::to_string(request.raw_request().size()));
  response->AddHeader("Content-Type", "text/plain");
  response->SetBody("HTTP/1.0 404 Not Found\r\n");
  return RequestHandler::OK;
}
