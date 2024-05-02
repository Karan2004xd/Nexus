#ifndef JSON_DATA_HANDLER_HPP
#define JSON_DATA_HANDLER_HPP

#include <rapidjson/document.h>
#include <string>
#include <unordered_map>
#include <vector>

class JsonDataHandler {
protected:
  typedef std::unordered_map<std::string, std::string> decodedJson;

  std::string encodeStringToJson(std::string &);
  decodedJson decodeJsonToString(std::string &);

private:
  void parseDocument(rapidjson::Document &, const std::string &);
  std::vector<std::string> getListValues(const rapidjson::Value::ConstArray &);
};
#endif // JSON_DATA_HANDLER_HPP
