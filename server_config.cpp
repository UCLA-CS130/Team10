#include "server_config.hpp"

ServerConfig::ServerConfig()
{

}

bool ServerConfig::Init(const NginxConfig* config)
{
  for ( auto statement : config->statements_ )
  {
    for (auto token: statement->tokens_)
    {
      int i = 0;
      if (token == "port")
        m_params["port"] = statement->tokens_[i + 1];
      if (token == "echo")
        m_params["echo_path"] = statement->tokens_[i + 1];
      if (token == "root")
        m_params["root"] = statement->tokens_[i + 1];
      if (token == "static")
        m_params["static_path"] = statement->tokens_[i + 1];
      i++;
      // In the server configuration context
      if (token == "server")
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
      }
    }
  }
  return true;
}

std::string ServerConfig::Root() const
{
  // TODO: error handling
  return m_params.at("root");
}

std::string ServerConfig::Port() const
{
  return m_params.at("port");
}

std::string ServerConfig::Echo() const
{
  return m_params.at("echo_path");
}

std::string ServerConfig::Static() const
{
  return m_params.at("static_path");
}
