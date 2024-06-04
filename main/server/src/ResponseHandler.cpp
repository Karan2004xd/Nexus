#include "../include/ResponseHandler.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"

#include <boost/beast.hpp>
#include <boost/beast/version.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

void ResponseHandler::handleResponse(std::string &jsonData,
                                     tcp::socket &socket) {
  auto decodedJson = Utils::SimpleJsonParser::decodeJson(jsonData);

  std::string body = std::get<std::string>(decodedJson.at("body"));
  std::string contentType = std::get<std::string>(decodedJson.at("content-type"));
  unsigned int version = std::stoi(std::get<std::string>(decodedJson.at("version")));
  http::status status;

  std::string tempStatus = std::get<std::string>(decodedJson.at("status"));

  if (tempStatus == "ok") {
    status = http::status::ok;
  } else if (tempStatus == "not_found") {
    status = http::status::not_found;
  } else if (tempStatus == "bad_request") {
    status = http::status::bad_request;
  }
  
  http::response<http::string_body> response;
  response.version(version);
  response.result(status);
  response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  response.set(http::field::content_type, contentType);
  response.body() = std::move(body);
  response.prepare_payload();
  http::write(socket, response);
}
