//
// not_found_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "not_found_handler.hpp"
#include <fstream>
#include <sstream>

NotFoundHandler::NotFoundHandler()
{

}

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix,
                      const ServerConfig& config)
{
  return RequestHandler::OK;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request,
                               Response* response)
{
  // TODO: Make the line below work
  // response = reply::stock_reply(reply::not_found);
  return RequestHandler::OK;
}
