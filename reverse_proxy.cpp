#include "reverse_proxy.hpp"

RequestHandler::Status ReverseProxy::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  m_uri_prefix; = uri_prefix;
  m_remote_host = "";
  m_remote_port = "";

  for ( auto statement : config.statements_ )
  {
    for (auto token: statement->tokens_)
    {
      if (token == "remote_host" && statement->tokens_.size() == 2)
      {
        m_remote_host = statement->tokens_[1];
      }
      else if (token == "remote_port" && statement->tokens_.size() == 2)
      {
        m_remote_port = statement->tokens_[1];
      }

    }
  }

  // Need both the host and the port config values
  if (remote_host.empty() || remote_port.empty())
  {
    return RequestHandler::INVALID;
  }

  return RequestHandler::OK;
}

RequestHandler::Status ReverseProxy::HandleRequest(const Request& request,
                               Response* response)
{ 
  return RequestHandler::OK;
}
