#include "../../include/http-requests/Post.hpp"

httplib::Request Post::getParsedBody(const std::string &body) {
  httplib::Request req {body};
  std::cout << req.method << std::endl;
  return req;
}

std::string Post::parseFileInputRequest(const std::string &body) {
  auto request = getParsedBody(body);
}

Post &parseSimpleFromDataRequest(const std::string &body);
