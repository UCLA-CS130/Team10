#ifndef LOG_H
#define LOG_H

#include <vector>
#include <string>
#include <iostream>

struct Record{
  std::string request_url;
  std::string response_code;
  //std::string handler_name;
  //std::string url_prefix;
};

class Log{
public:
  static Log *instance();

  void add_record(const std::string& url, const std::string& response_code);// const std::string& handler_name, const std::string& url_prefix);

  std::string records() const;

private:
  static Log *m_instance;
  std::vector<Record> m_records;
};

#endif