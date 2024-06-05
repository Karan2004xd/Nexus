#ifndef POST_HPP
#define POST_HPP
#include <httplib.h>
#include <string>

class Post {
public:
  Post() {}
  std::string parseFileInputRequest(const std::string &body);
  std::string parseSimpleFromDataRequest(const std::string &body);
private:
  httplib::Request getParsedBody(const std::string &body);
};

#endif // POST_HPP
