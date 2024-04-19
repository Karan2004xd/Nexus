#ifndef SERVER_HANDLER_HPP
#define SERVER_HANDLER_HPP

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <string>
#include <fstream>
#include <iostream>

#include "../../../constants.h"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace logging = boost::log;

using tcp = net::ip::tcp;

class ServerHandler {
public:
  ServerHandler();
  void startListening();
private:
  // Setup connection and some default setting for the server to operate
  net::io_context ioContext;
  tcp::socket socket {ioContext};
  tcp::acceptor acceptor {ioContext, {tcp::v4(), DEFAULT_PORT}};

  std::string previousPath;

  std::string getRequestTarget(const http::request<http::string_body> &);
  bool endsWith(const std::string &, const std::string &);
  std::string getMimeType(const std::string &);
  std::string readFile(const std::string &);

  void handleRequest(const http::request<http::string_body> &);
  void handleResponse(const http::request<http::string_body> &, const std::string &);
};
#endif // SERVER_HANDLER_HPP
