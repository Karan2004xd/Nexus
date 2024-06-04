#ifndef CONNECTION_HANDLER_HPP
#define CONNECTION_HANDLER_HPP

#include "RequestHandler.hpp"
#include "ResponseHandler.hpp"
#include <boost/asio/ip/tcp.hpp>

class ConnectionHandler : protected RequestHandler, ResponseHandler {
public:
  static void startListener();

  static void handleRequest(const http::request<http::string_body> &request,
                            boost::asio::ip::tcp::socket &socket);
};
#endif // CONNECTION_HANDLER_HPP
