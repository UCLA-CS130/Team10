#include "reverse_proxy.hpp"

RequestHandler::Status ReverseProxy::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
    m_uri_prefix; = uri_prefix;
    remote_host = "";
    remote_port = "";

    for ( auto statement : config.statements_ ) {
        for (auto token: statement->tokens_) {
            
            if (token == "remote_host" && statement->tokens_.size() == 2) {
                remote_host = statement->tokens_[1];
            } else if (token == "remote_port" && statement->tokens_.size() == 2) {
                remote_port = statement->tokens_[1];
            }

        }
    }

    if (remote_host.empty() || remote_port.empty())
        return RequestHandler::INVALID;

  return RequestHandler::OK;
}

RequestHandler::Status ReverseProxy::HandleRequest(const Request& request,
                               Response* response)
{
  
  return RequestHandler::OK;
}
