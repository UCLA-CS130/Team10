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
#include "server_config.hpp"

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
    // TODO: Move this into ServerConfig
    NginxConfigParser *parser = new NginxConfigParser();
    NginxConfig *out_config = new NginxConfig();
    parser->Parse(argv[1], out_config);

    // TODO: Just pass in ServerConfig to the Server
    ServerConfig* server_config = new ServerConfig();
    if (server_config->Init(out_config))
    {
      // Initialise the server.
      std::string host = "0.0.0.0";
      std::string port = server_config->Port();
      std::string root = server_config->Root();

      std::string static_path = server_config->Static();
      std::string echo_path = server_config->Echo();

      server s(host, port, root, static_path, echo_path);

      // Run the server until stopped.
      std::cout << "The server is running on port " << port << std::endl;
      s.run();
    }
    else
    {
      std::cout << "failed." << std::endl;
    }

    // Clean up?
    delete parser;
    delete out_config;
    delete server_config;
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
