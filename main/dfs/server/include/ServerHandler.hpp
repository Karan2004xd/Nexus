#ifndef SERVER_HANDLER_HPP
#define SERVER_HANDLER_HPP

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>

#include "../../../constants.h"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

using tcp = net::ip::tcp;

class ServerHandler {
public:
  ServerHandler() {}
  void startListening();
private:
  // Setup connection and some default setting for the server to operate
  net::io_context ioContext;
  tcp::acceptor acceptor {ioContext, {tcp::v4(), DEFAULT_PORT}};

};
#endif // SERVER_HANDLER_HPP
