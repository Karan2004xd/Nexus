#ifndef SIMPLE_JSON_PARSER_HPP
#define SIMPLE_JSON_PARSER_HPP

#include "JsonParser.hpp"
#include "../../dfs/include/Nexus.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace Nexus;

class Utils::SimpleJsonParser : protected JsonParser {
protected:
  SimpleJsonParser() {}
  std::string encodeIntoJsonString(JsonDataParams &jsonDataparams) override;
  JsonDataParams decodeJsonString(std::string &jsonString) override;

public:
  static JsonParserBuilder JsonBuilder() {
    return JsonParser::JsonBuilder();
  }

  static std::string encodeJson(JsonDataParams &jsonDataparams,
                                SimpleJsonParser *parser = new SimpleJsonParser()) {
    return parser->encodeIntoJsonString(jsonDataparams);
  }

  static JsonDataParams decodeJson(std::string &jsonString,
                                   SimpleJsonParser *parser = new SimpleJsonParser()) {
    return parser->decodeJsonString(jsonString);
  }
private:
  std::vector<std::string> getListValues(const rapidjson::Value::ConstArray &dataList);
};
#endif // SIMPLE_JSON_PARSER_HPP
