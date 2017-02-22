#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "config_parser.hpp"


typedef std::unordered_map <std::string, std::string> Dictionary;

class ServerConfig {
public:
  ServerConfig();
  bool Init(const char* config_file);
  std::string Host() const;
  std::string Root() const;
  std::string Port() const;
  std::string Echo() const;
  std::string Static() const;
  std::string NotFound() const;
  std::string Status() const;


private:
  Dictionary m_params;
};

#endif // SERVER_CONFIG_HPP
