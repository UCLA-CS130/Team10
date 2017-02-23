#include "server_config.hpp"
#include "echo_handler.hpp"

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
      int i = 0;
      if (token == "port" && statement->tokens_.size() == 2)
        m_port = statement->tokens_[i + 1];
      if (token == "path" && statement->tokens_.size() == 3){
        //TODO: get uriprefix and create a handler
        std::string uri_prefix = statement->tokens_[i + 1];
        //std::string handler_name = statement->tokens_[i + 2];
        if(statement->child_block_->statements_.size() == 0){
          //TODO: create echo handler
          std::shared_ptr<RequestHandler> handler_ptr (new EchoHandler());
          m_handler_map[uri_prefix] = std::move(handler_ptr);
        }
        else{
          //TODO: create file handler
          //std::shared_ptr<RequestHandler> handler_ptr (new StaticHandler());
          //m_handler_map[uri_prefix] = std::move(handler_ptr);
        }
      }
      if (token == "default" && statement->tokens_.size() == 2){
        //TODO: get urlprefix and create a NotFound Handler
        std::string uri_prefix = statement->tokens_[i + 1];
        //std::string handler_name = statement->tokens_[i + 2];
        //m_params["root"] = statement->tokens_[i + 1];
      }

      //if (token == "static")
        //m_params["static_path"] = statement->tokens_[i + 1];
      i++;
      // In the server configuration context
      /*if (token == "server")
      {
        // Open NginxConfig object
        NginxConfig* serverConfig = statement->child_block_.get();
        for (auto stmt : serverConfig->statements_ )
        {
          int index = 0;
          for (auto& tkn : stmt->tokens_)
          {
            if (tkn == "listen")
            {
              m_params["port"] = stmt->tokens_[index + 1];
            }

            if (tkn == "server_name")
            {
              m_params["host"] = stmt->tokens_[index + 1];
            }

            if (tkn == "path")
            {
              if (stmt->tokens_[index + 1] == "/echo")
              {
                m_params["echo_path"] = stmt->tokens_[index + 1];
                continue;
              }
              else if (stmt->tokens_[index + 1] == "/static")
              {
                NginxConfig* loc = stmt->child_block_.get();
                for (auto s : loc->statements_)
                {
                  int i = 0;
                  for (auto t : s->tokens_)
                  {
                    if (t == "root")
                    {
                      m_params["root"] = s->tokens_[i + 1];
                      m_params["static_path"] = stmt->tokens_[index + 1];
                    }
                  }
                  i++;
                }
              }
            }
            index++;
          }
        }
      }*/
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


HandlerMap ServerConfig::Handler_map() const
{
  return m_handler_map;
}
