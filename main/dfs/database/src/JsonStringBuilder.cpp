#include "../include/JsonStringBuilder.hpp"

using namespace Database;

JsonStringBuilder::JsonStringBuilder() {
  jsonData.clear();
  jsonData << "{";
}

JsonStringBuilder &JsonStringBuilder::singleData(const std::string &name,
                                                 const std::string &value) {
  if (jsonData.str().length() > 2) {
    jsonData << ", ";
  }
  jsonData << "\"" << name << "\"" << ": \"" << value << "\"";
  return *this;
}

JsonStringBuilder &JsonStringBuilder::listOfData(const std::string &name,
                                                 const std::vector<std::string> &values) {
  if (jsonData.str().length() > 2) {
    jsonData << ", ";
  }

  std::string valuesData;
  for (int i = 0; i < values.size(); i++) {
    valuesData += "\"" + values[i] + "\"";
    if (i < values.size() - 1) {
      valuesData += ", ";
    }
  }
  jsonData << "\"" << name << "\": [" << valuesData << "]";
  return *this;
}

void JsonStringBuilder::build() {
  jsonData << "}";
}
