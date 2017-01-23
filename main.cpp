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
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"

#include "config_parser.hpp"

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

    // out_config is a list of vectors representing statements
    // Iterate through all statements
    // Construct a server object to hold params
    // Pass object.params into server constructor
    // http::server::server s(server.name,server.port, server.location);

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
    // http::server::server s('0.0.0.0', argv[2], argv[3]);
    //
    // // Run the server until stopped.
    // s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
