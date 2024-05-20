#include "../include/ConnectionHandler.hpp"
#include "../include/Logging.hpp"
#include "../../constants.h"

#include <boost/beast.hpp>
#include <boost/asio.hpp>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

using tcp = net::ip::tcp;

template <typename T1>
void ConnectionHandler::handleResponse(T1 &socket, unsigned int requestVersion) {

}

template <typename T1, typename T2>
void ConnectionHandler::handleRequest(const T1 &request, T2 &socket) {
  std::string requestTarget = request.target();
  std::string requestMethod = request.method_string();

  /* handleResponse(socket, request.version()); */
}

void ConnectionHandler::startListener() {
  try {
    BOOST_LOG_TRIVIAL(info) << "Initiating server at port: " << DEFAULT_PORT;
    net::io_context ioContext;
    tcp::acceptor acceptor {ioContext, {tcp::v4(), DEFAULT_PORT}};

    BOOST_LOG_TRIVIAL(info) << "Server initialized properly.";
    BOOST_LOG_TRIVIAL(info) << "Running on port: " << DEFAULT_PORT;

    while (true) {
      tcp::socket socket {ioContext};
      acceptor.accept(socket);

      beast::flat_buffer buffer;
      http::request<http::string_body> request;
      http::read(socket, buffer, request);
      
      std::thread([&]() mutable {
        ConnectionHandler::handleRequest(request, socket);
      }).detach();
    }
  } catch (std::exception &e) {
    // Create a different response containing the error
  }
}
