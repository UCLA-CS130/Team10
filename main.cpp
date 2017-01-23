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

struct Server {
  std::unordered_map <std::string, std::string> params;
};

// TODO: Change to recursive if needed?
bool parseNginxConfig(NginxConfig* config, Server* out) 
{

  for ( auto statement : config->statements_ ) 
  {
    std::cout << statement->ToString(0) << std::endl;
    for (auto token: statement->tokens_) 
    {
      std::cout << token << std::endl;

      // In the server configuration context
      if (token == "server") 
      {
        // TODO: Recurse here to generate a child Server object
        // parserNginxConfig(statement->child_block_, child)
        // Open NginxConfig object
        NginxConfig* serverConfig = statement->child_block_.get();
        for (auto stmt : serverConfig->statements_ )
        {
          int index = 0;
          for (auto& tkn : stmt->tokens_)
          {
            std::cout << tkn << std::endl;

            if (tkn == "listen") 
            {
              out->params["port"] = stmt->tokens_[index + 1];
            }

            if (tkn == "server_name") 
            {
              out->params["host"] = stmt->tokens_[index + 1];
            }

            if (tkn == "location")
            {
              // In location context
              NginxConfig* loc = stmt->child_block_.get();
              for (auto s : loc->statements_)
              {
                int i = 0;
                for (auto t : s->tokens_)
                {
                  std::cout << t << std::endl;
                  if (t == "root")
                  {
                    out->params["root"] = s->tokens_[i + 1];
                  }
                }
                i++;
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

    // TODO: Take in one command-line argument for config file
    if (argc != 2)
    {
      std::cerr << "Usage: ./server /path/to/config/file\n";
      return 1;
    }

    // Initialise the NginxConfigParser
    NginxConfigParser *parser = new NginxConfigParser();
    NginxConfig *out_config = new NginxConfig();

    parser->Parse(argv[1], out_config);

    Server* server = new Server();

    parseNginxConfig(out_config, server );

    std::cout << server->params["host"] << std::endl;
    std::cout << server->params["port"] << std::endl;
    std::cout << server->params["root"] << std::endl;

    // out_config is a list of vectors representing statements
    // Iterate through all statements
    // Construct a server object to hold params
    // Pass object.params into server constructor
    // http::server::server s(server.name,server.port, server.location);

    // OR 

    // Pass directly as config file and let server class handle further parsing
    // http::server::server s(out_config);

    // UNCOMMENT BELOW FOR REGULAR OPERATION

    // Check command line arguments.
    // if (argc != 4)
    // {
    //   std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
    //   std::cerr << "  For IPv4, try:\n";
    //   std::cerr << "    receiver 0.0.0.0 80 .\n";
    //   std::cerr << "  For IPv6, try:\n";
    //   std::cerr << "    receiver 0::0 80 .\n";
    //   return 1;
    // }
    //
    // Initialise the server.
    std::string host = server->params["host"];
    std::string port = server->params["port"];
    std::string root = server->params["root"];
    http::server::server s(host, port, root);
    
    // Run the server until stopped.
    s.run();

    // Delete
    delete parser;
    delete out_config;
    delete server;
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }



  return 0;
}
