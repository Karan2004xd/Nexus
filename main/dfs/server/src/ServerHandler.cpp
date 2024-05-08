#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <iostream>
#include <fstream>

#include "../include/ServerHandler.hpp"
#include "../../../constants.h"

namespace logging = boost::log;
ServerHandler::ServerHandler() { 
  logging::core::get()->set_logging_enabled(true);
}

std::string ServerHandler::getRequestTarget(const http::request<http::string_body> &request) {
  std::string target;
  if (request.target().empty() || request.target()[0] != '/') {
    target = "/";
  } else {
    target = std::string(request.target().data(), request.target().size());
  }
  return target;
}

bool ServerHandler::endsWith(const std::string &str, const std::string &suffix) {
  if (suffix.length() > str.length()) return false;
  return str.substr(str.length() - suffix.length()) == suffix;
}

std::string ServerHandler::readFile(const std::string &path) {
  std::ifstream file {path, std::ios::binary};
  if (!file.is_open()) {
    BOOST_LOG_TRIVIAL(warning) << "File at path (" << path << ") was not found";
  }
  std::string fileContent {(std::istreambuf_iterator<char> (file)), std::istreambuf_iterator<char>()};
  return fileContent;
}

std::string ServerHandler::getMimeType(const std::string &path) {
  if (endsWith(path, ".html")) return "text/html";
  if (endsWith(path, ".css")) return "text/css";
  if (endsWith(path, ".js")) return "text/javascript";
  return "application/octet_stream"; // Default MIME Type
}

void ServerHandler::handleResponse(const http::request<http::string_body> &request,
                                   const std::string &path,
                                   tcp::socket &socket
                                   ) {
  http::response<http::string_body> response;
  response.version(request.version());
  response.result(http::status::ok);
  response.set(http::field::server, "Nexus Server");
  response.set(http::field::content_type, getMimeType(path));

  BOOST_LOG_TRIVIAL(info) << "Sending resposne";

  try {
    std::string fileContent = readFile(path);
    response.body() = std::move(fileContent);
    response.prepare_payload();
  } catch (const std::exception &e) {
    response.result(http::status::not_found);
    response.body() = "404 Not Found";
    response.prepare_payload();
  }
  http::write(socket, response);

  BOOST_LOG_TRIVIAL(info) << "Resposne sent";
}

void ServerHandler::handleRequest(const http::request<http::string_body> &request,
                                  tcp::socket &socket) {
  std::string requestTarget = getRequestTarget(request);

  std::string pathToInterfaces = std::string(PATH_TO_INTERFACES);
  std::string path;

  if (request.method() == http::verb::post) {
    path = previousPath;
    auto contentTypeIt = request.find(http::field::content_type);
    std::string contentType;

    if (contentTypeIt != request.end()) {
      contentType = contentTypeIt->value();
      std::cout << contentType << std::endl;
      std::cout << request.body() << std::endl;
    }
  } else {
    if (requestTarget == "/") {
      path = pathToInterfaces + "/index.html";
    } else if (endsWith(requestTarget, ".html")) {
      path = pathToInterfaces + requestTarget;
    } else if (endsWith(requestTarget, ".js")) {
      path = pathToInterfaces + requestTarget;
    } else if (endsWith(requestTarget, ".css")) {
      path = pathToInterfaces + requestTarget;;
    }
    previousPath = path;

    BOOST_LOG_TRIVIAL(info) << "Running File from path " << path;
    handleResponse(request, path, socket);
  }
}

void ServerHandler::startListening() {
  BOOST_LOG_TRIVIAL(info) << "Server is active Running at port " << DEFAULT_PORT;
  try {
    net::io_context ioContext;
    tcp::acceptor acceptor {ioContext, {tcp::v4(), DEFAULT_PORT}};
    BOOST_LOG_TRIVIAL(info) << "Socket connected: ";
    BOOST_LOG_TRIVIAL(info) << "Reading data now...";

    while (true) {
      tcp::socket socket {ioContext};
      acceptor.accept(socket);

      beast::flat_buffer buffer;
      http::request<http::string_body> request;
      http::read(socket, buffer, request);

      handleRequest(request, socket);
      /* std::thread([this]() mutable { */
      /* }).detach(); */
    }
  } catch (std::exception &e) {
    BOOST_LOG_TRIVIAL(fatal) << "Exception: " << e.what();
  }
}
