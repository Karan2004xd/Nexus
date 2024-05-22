#ifndef JSON_PARSER_BUILDER_HPP
#define JSON_PARSER_BUILDER_HPP

#include <initializer_list>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>

class JsonParserBuilder {
protected:
  JsonParserBuilder() {};
  static JsonParserBuilder build() { return {}; }

public:
  typedef std::unordered_map<std::string, std::variant<std::string ,std::vector<std::string>>> JsonDataParams;
  JsonParserBuilder singleData(const std::string &name,
                               const std::string &value);

  JsonParserBuilder multipleData(const std::string &name,
                                 const std::initializer_list<std::string> &values);
  
  JsonDataParams getJsonData() const { return this->jsonData; }

private:
  JsonDataParams jsonData;
  
  void checkIfKeyPresent(const std::string &key) const {
    if (jsonData.find(key) != jsonData.end()) {
      throw std::runtime_error("(JsonParserBuilder) : Error in checkIfKeyPresent, key is already present");
    }
  }

  template <typename T>
  void addJsonDataHelper(const std::string &name,
                         const T &value);

  void addJsonData(const std::string &name,
                   const std::string &value);

  void addJsonData(const std::string &name,
                   const std::initializer_list<std::string> &values);
};
#endif // JSON_PARSER_BUILDER_HPP
