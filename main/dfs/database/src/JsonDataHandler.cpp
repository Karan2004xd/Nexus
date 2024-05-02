#include "../include/JsonDataHandler.hpp"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

void JsonDataHandler::parseDocument(rapidjson::Document &document, 
                                    const std::string &data) {
  if (document.Parse(data.c_str()).HasParseError()) {
    throw std::runtime_error("Json Data Parsing Error: (" + data + ") cannot be parsed");
  }
}

std::vector<std::string> JsonDataHandler::getListValues(const rapidjson::Value::ConstArray &dataList) {
  std::vector<std::string> resultList;
  for (rapidjson::SizeType i = 0; i < dataList.Size(); i++) {
    resultList.push_back(dataList[i].GetString());
  }
  return resultList;
}

std::string JsonDataHandler::encodeStringToJson(std::string &stringData) {
  rapidjson::Document documentToEncode;
  parseDocument(documentToEncode, stringData);

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer {buffer};

  documentToEncode.Accept(writer);
  return buffer.GetString();
}

JsonDataHandler::decodedJson JsonDataHandler::decodeJsonToString(std::string &jsonData) {
  JsonDataHandler::decodedJson resultDecodedJson;
  rapidjson::Document documentToDecode;

  parseDocument(documentToDecode, jsonData);
  for (rapidjson::Value::ConstMemberIterator itr = documentToDecode.MemberBegin(); 
        itr != documentToDecode.MemberEnd(); itr++) {

    if (itr->value.IsString()) {
      resultDecodedJson[itr->name.GetString()] = itr->value.GetString();
    }
  }
  return resultDecodedJson;
}
