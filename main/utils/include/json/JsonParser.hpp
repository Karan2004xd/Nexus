#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include "JsonParserBuilder.hpp"

class JsonParser : protected JsonParserBuilder {
protected:
  static JsonParserBuilder JsonBuilder() {
    return JsonParserBuilder::build();
  }
  virtual std::string encodeIntoJsonString(JsonDataParams &jsonDataparams) = 0;
  virtual JsonDataParams decodeJsonString(std::string &jsonString) = 0;
};
#endif // JSON_PARSER_HPP
