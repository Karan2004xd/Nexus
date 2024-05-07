#include <boost/beast.hpp>
#include <boost/asio.hpp>

/* #include <boost/log/core.hpp> */
/* #include <boost/log/trivial.hpp> */
/* #include <boost/log/expressions.hpp> */

#include <fstream>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
/* namespace logging = boost::log; */

using tcp = net::ip::tcp;

bool endsWith(const std::string& str, const std::string& suffix) {
  if (str.length() < suffix.length()) return false;
  return str.substr(str.length() - suffix.length()) == suffix;
}

std::string getMimeType(const std::string& path) {
  if (endsWith(path, ".html")) return "text/html";
  if (endsWith(path, ".js")) return "text/javascript";
  if (endsWith(path, ".css")) return "text/css";
  return "application/octet-stream"; // Default MIME type
}

std::string readFile(const std::string& path) {
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open()) {
    /* BOOST_LOG_TRIVIAL(fatal) << "Failed to open file"; */
    throw std::runtime_error("Failed to open file: " + path);
  }
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return content;
}

std::string getRequestTarget(const http::request<http::string_body>& request) {
  std::string target;
  if (request.target().empty() || request.target()[0] != '/') {
    target = "/";
  } else {
    target = std::string(request.target().data(), request.target().size());
    /* std::cout << target << std::endl; */
  }
  return target;
}

int main() {
  try {
    /* logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info); */
    net::io_context io_context;
    tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});
    std::cout << "Listening for request" << std::endl;

    while (true) {
      tcp::socket socket(io_context);
      acceptor.accept(socket);
      /* BOOST_LOG_TRIVIAL(info) << "Socket connected"; */

      beast::flat_buffer buffer;
      http::request<http::string_body> request;
      http::read(socket, buffer, request);
      std::cout << request.body() << std::endl;

      /* BOOST_LOG_TRIVIAL(info) << "Data Read"; */

      if (request.method() == http::verb::post) {
        /* BOOST_LOG_TRIVIAL(info) << request.body(); */
      } else {
        std::string path = "." + getRequestTarget(request);
        if (path == "./") {
          path += "index.html"; // Serve index.html by default
        }
        std::cout << path << std::endl;
        http::response<http::string_body> response;
        response.version(request.version());
        response.result(http::status::ok);
        response.set(http::field::server, "Boost Beast Server");
        response.set(http::field::content_type, getMimeType(path));

        try {
          std::string fileContent = readFile(path);
          response.body() = std::move(fileContent);
          response.prepare_payload();
        } catch (const std::exception& e) {
          response.result(http::status::not_found);
          response.body() = "404 Not Found";
          response.prepare_payload();
        }
        http::write(socket, response);
      }
    }

  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}

