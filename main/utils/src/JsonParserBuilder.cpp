#include "../include/JsonParserBuilder.hpp"

template <typename T>
void JsonParserBuilder::addJsonDataHelper(const std::string &name,
                                    const T &value) {
  checkIfKeyPresent(name);
  jsonData[name] = value;
}

void JsonParserBuilder::addJsonData(const std::string &name,
                                    const std::string &value) {
  addJsonDataHelper(name, value);
}

void JsonParserBuilder::addJsonData(const std::string &name,
                                    const std::initializer_list<std::string> &values) {
  addJsonDataHelper(name, values);
}

JsonParserBuilder JsonParserBuilder::singleData(const std::string &name,
                                                const std::string &value) {
  addJsonData(name, value);
  return *this;
}

JsonParserBuilder JsonParserBuilder::multipleData(const std::string &name,
                                                  const std::initializer_list<std::string> &values) {
  addJsonData(name, values);
  return *this;
}
