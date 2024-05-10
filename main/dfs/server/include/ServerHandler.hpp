#ifndef SERVER_HANDLER_HPP
#define SERVER_HANDLER_HPP

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <string>
#include "../../utility/include/JsonDataHandler.hpp"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

using tcp = net::ip::tcp;

class ServerHandler : protected Utility::JsonDataHandler {
public:
  ServerHandler();
  void startListening();
private:
  std::string previousPath;

  std::string getRequestTarget(const http::request<http::string_body> &);
  bool endsWith(const std::string &, const std::string &);
  std::string getMimeType(const std::string &);
  std::string readFile(const std::string &);

  std::string getResponse(decodedJson &);

  void handleRequest(const http::request<http::string_body> &,
                     tcp::socket &);

  void handleResponse(const http::request<http::string_body> &,
                      const std::string &,
                      tcp::socket &);

  void handleResponse(const std::string &,
                      decodedJson &,
                      tcp::socket &,
                      unsigned int);
};
#endif // SERVER_HANDLER_HPP
