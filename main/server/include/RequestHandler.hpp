#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <boost/beast/http/status.hpp>
#include <string>
#include <unordered_map>
#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../dfs/include/Dfs.hpp"

#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

class RequestHandler : protected Nexus::Dfs {
protected:
  typedef boost::beast::http::status StatusCode;
  enum RequestParams {
    CONTENT_TYPE,
    BODY,
    VERSION,
    STATUS
  };
  typedef std::unordered_map<RequestParams, std::variant<std::string, unsigned int, StatusCode, 
  std::unordered_map<std::string, std::vector<std::string>>>> RequestMap;
  
  static std::string handleRequest(const http::request<http::string_body> &request,
                                   RequestHandler *reqObj = {}) {
    return reqObj->handleRequestHelper(request);
  }
private:
  std::string handleRequestHelper(const http::request<http::string_body> &request);

  typedef Utils::SimpleJsonParser::JsonDataParams JsonDataParams;

  std::string getRequestTarget(const std::string &requestTarget);
  bool endsWith(const std::string &str, const std::string &suffix);

  std::string getStaticFileContent(const std::string &fileName);
  std::string getMimeType(const std::string &arg);

  std::string getContentType(const http::request<http::string_body> &request);
  bool checkJsonData(const std::initializer_list<std::string> &jsonArgs,
                     const JsonDataParams &jsonParams);

  RequestMap getRequestMap(const JsonDataParams &jsonParams,
                           const std::string &contentType,
                           const unsigned int &version);

  std::string getResponseData(const RequestMap &requestMap);
  std::string getResponseData(const std::string &target,
                              const unsigned int &version);
};
#endif // REQUEST_HANDLER_HPP
