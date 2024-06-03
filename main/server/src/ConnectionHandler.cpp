#include "../include/ConnectionHandler.hpp"
#include "../include/Logging.hpp"
#include "../../constants.h"

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

using tcp = net::ip::tcp;

void ConnectionHandler::handleRequest(const http::request<http::string_body> &request,
                                      tcp::socket &socket) {
  std::string jsonRequest = RequestHandler::handleRequest(request);
  ResponseHandler::handleResponse(jsonRequest, socket);
}

void ConnectionHandler::startListener() {
  try {
    BOOST_LOG_TRIVIAL(info) << "Initiating server at port: " << DEFAULT_PORT;
    net::io_context ioContext;
    tcp::acceptor acceptor {ioContext, {tcp::v4(), DEFAULT_PORT}};

    BOOST_LOG_TRIVIAL(info) << "Server initialized properly.";
    BOOST_LOG_TRIVIAL(info) << "Running on port: " << DEFAULT_PORT;

    while (true) {
      try {
        tcp::socket socket {ioContext};
        acceptor.accept(socket);

        beast::flat_buffer buffer;
        http::request<http::string_body> request;
        http::read(socket, buffer, request);

        ConnectionHandler::handleRequest(request, socket);
      } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        continue;
      }
    }
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
