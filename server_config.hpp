#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "config_parser.hpp"
#include "request_handler.hpp"
#include "log.hpp"


class ServerConfig {
public:
  // Constructor
  ServerConfig();

  // Initialize the class
  // return true if config_file is legal. Otherwise, return false.
  bool Init(const char* config_file);

  // get port number
  std::string Port() const;


private:
  // Port number
  std::string m_port;
};

#endif // SERVER_CONFIG_HPP
