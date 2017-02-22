//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"

#include "config_parser.hpp"

// I don't wanna type std::unordered_map blahblahba
typedef std::unordered_map <std::string, std::string> Dictionary;
struct Server {
  Dictionary params;
};

// TODO: Change to recursive if needed?
bool parseNginxConfig(NginxConfig* config, Server* out)
{
  for ( auto statement : config->statements_ )
  {
    for (auto token: statement->tokens_)
    {
      int i = 0;
      if (token == "port")
        out->params["port"] = statement->tokens_[i + 1];
      if (token == "echo")
        out->params["echo_path"] = statement->tokens_[i + 1];
      if (token == "root")
        out->params["root"] = statement->tokens_[i + 1];
      if (token == "static")
        out->params["static_path"] = statement->tokens_[i + 1];
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
              out->params["port"] = stmt->tokens_[index + 1];
            }

            if (tkn == "server_name")
            {
              out->params["host"] = stmt->tokens_[index + 1];
            }

            if (tkn == "path")
            {
              if (stmt->tokens_[index + 1] == "/echo")
              {
                out->params["echo_path"] = stmt->tokens_[index + 1];
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
                      out->params["root"] = s->tokens_[i + 1];
                      out->params["static_path"] = stmt->tokens_[index + 1];
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

int main(int argc, char* argv[])
{
  try
  {

    if (argc != 2)
    {
      std::cerr << "Usage: ./webserver /path/to/config/file\n";
      return 1;
    }

    // Initialise the NginxConfigParser
    NginxConfigParser *parser = new NginxConfigParser();
    NginxConfig *out_config = new NginxConfig();

    parser->Parse(argv[1], out_config);
    Server* server_param = new Server();
    parseNginxConfig(out_config, server_param);

    //TODO: Determine echo server or file server

    // Initialise the server.
    std::string host = "0.0.0.0";
    std::string port = server_param->params["port"];
    std::string root = server_param->params["root"];

    std::string static_path = server_param->params["static_path"];
    std::string echo_path = server_param->params["echo_path"];

    server s(host, port, root, static_path, echo_path);

    // Run the server until stopped.
    std::cout << "The server is running on port " << port << std::endl;
    s.run();

    // Clean up
    delete parser;
    delete out_config;
    delete server_param;
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
