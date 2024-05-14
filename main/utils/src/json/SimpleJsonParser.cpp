#include "../../include/json/SimpleJsonParser.hpp"
#include <sstream>
#include <stdexcept>

using namespace rapidjson;
using namespace Utils;

std::string SimpleJsonParser::encodeIntoJsonString(JsonDataParams &jsonDataparams) {
  Document document;
  document.SetObject();

  for (const auto &name : jsonDataparams) {
    Value valueParam;
    if (std::holds_alternative<std::string>(name.second)) {
      std::string valueData = std::get<std::string>(name.second);
      valueParam.SetString(valueData.c_str(), document.GetAllocator());

    } else if (std::holds_alternative<std::vector<std::string>>(name.second)) {
      valueParam = Value(kArrayType);
      auto vec = std::get<std::vector<std::string>>(name.second);

      for (const auto &vectorValue : vec) {
        valueParam.PushBack(GenericStringRef(vectorValue.c_str()), document.GetAllocator());
      }
    }
    GenericStringRef nameValue(name.first.c_str());
    document.AddMember(nameValue, valueParam, document.GetAllocator());
  }
  StringBuffer buffer;
  Writer<StringBuffer> writer {buffer};
  document.Accept(writer);

  return buffer.GetString();
}

std::vector<std::string> SimpleJsonParser::getListValues(const Value::ConstArray &dataList) {
  std::vector<std::string> resultList;
  for (SizeType i = 0; i < dataList.Size(); i++) {
    resultList.push_back(dataList[i].GetString());
  }
  return resultList;
}

SimpleJsonParser::JsonDataParams SimpleJsonParser::decodeJsonString(std::string &jsonString) {
  JsonDataParams decodedJsonData;

  Document document;
  document.Parse(jsonString.c_str());
  std::ostringstream oss;

  if (document.HasParseError()) {
    oss << "(SimpleJsonParser) : Error in decodeJsonString, "
        << "while parsing ( " << jsonString << ") ";
    throw std::runtime_error(oss.str());
  }
  
  for (Value::ConstMemberIterator itr = document.MemberBegin();
        itr != document.MemberEnd(); itr++) {
    if (itr->value.IsString()) {
      decodedJsonData[itr->name.GetString()] = itr->value.GetString();
    } else if (itr->value.IsArray()) {
      decodedJsonData[itr->name.GetString()] = getListValues(itr->value.GetArray());
    }
  }
  return decodedJsonData;
}
