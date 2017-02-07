//
// request_handler_file.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_FILE_HPP
#define HTTP_REQUEST_HANDLER_FILE_HPP

#include <string>
#include "request_handler.hpp"

namespace http {
namespace server {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler_file : public http::server::request_handler
{
public:
  /// Construct with a directory containing files to be served.
  explicit request_handler_file(const std::string& doc_root, const std::string& echo_path);

  /// Handle a request and produce a reply.
  void handle_request(const request& req, reply& rep) override;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);

private:
  /// The directory containing the files to be served.
  std::string doc_root_;
  std::string echo_path_;
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_FILE_HPP
