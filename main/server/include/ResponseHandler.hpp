#ifndef RESPONSE_HANDLER_HPP
#define RESPONSE_HANDLER_HPP
#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace net = boost::asio;

using tcp = net::ip::tcp;

class ResponseHandler {
protected:
  static void handleResponse(std::string &jsonData,
                             tcp::socket &socket);
};
#endif // RESPONSE_HANDLER_HPP
