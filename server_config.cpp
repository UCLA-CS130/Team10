#include "server_config.hpp"
#include "echo_handler.hpp"
#include "not_found_handler.hpp"
#include "static_handler.hpp"
#include "status_handler.hpp"
#include <iostream> // debug purpose

ServerConfig::ServerConfig()
{

}

bool ServerConfig::Init(const char* config_file)
{

  NginxConfigParser *parser = new NginxConfigParser();
  NginxConfig *config = new NginxConfig();
  parser->Parse(config_file, config);

  for ( auto statement : config->statements_ )
  {
    for (auto token: statement->tokens_)
    {
      // Extract port number
      if (token == "port" && statement->tokens_.size() == 2)
        m_port = statement->tokens_[1];

      // Create echo handler or static handler
      else if (token == "path" && statement->tokens_.size() == 3){
        // TODO: catch exception

        // Extract uri_prefix and handler_name
        std::string uri_prefix = statement->tokens_[1];
        std::string handler_name = statement->tokens_[2];

        // Create handler by name
        auto raw_handler_ptr = RequestHandler::CreateByName(handler_name.c_str());
        std::shared_ptr<RequestHandler> handler_ptr (raw_handler_ptr);

        if(handler_ptr->Init(uri_prefix, *(statement->child_block_)) == RequestHandler::OK)
          Log::instance()->add_mapping(uri_prefix, handler_name, handler_ptr);
      }
      else if (token == "default" && statement->tokens_.size() == 2){
        //TODO: catech exception

        std::string uri_prefix = "";
        std::string handler_name = statement->tokens_[1];
        auto raw_handler_ptr = RequestHandler::CreateByName(handler_name.c_str());
        std::shared_ptr<RequestHandler> handler_ptr (raw_handler_ptr);
        handler_ptr->Init(uri_prefix, *(statement->child_block_));
        Log::instance()->add_mapping(uri_prefix, handler_name, handler_ptr);
      }
    }
  }

  delete parser;
  delete config;
  return true;
}


std::string ServerConfig::Port() const
{
  return m_port;
}


/*HandlerMap ServerConfig::Handler_map() const
{
  return m_handler_map;
}*/
