#include "../include/ServerHandler.hpp"

void ServerHandler::startListening() {
  try {
    while (true) {
      tcp::socket socket {ioContext};
      acceptor.accept(socket);
      std::cout << "Socket connected: " << "Running at port " << DEFAULT_PORT << std::endl;
      std::cout << "Reading data now..." << std::endl;
      /* BOOST_LOG_TRIVIAL(info) << "Socket connected: Running at port " << DEFAULT_PORT; */
      /* BOOST_LOG_TRIVIAL(info) << "Reading data now..."; */

      beast::flat_buffer buffer;
      http::request<http::string_body> request;
      http::read(socket, buffer, request);

    }
  } catch (std::exception &e) {
    /* BOOST_LOG_TRIVIAL(fatal) << e.what(); */
    std::cout << e.what() << std::endl;
  }
}
