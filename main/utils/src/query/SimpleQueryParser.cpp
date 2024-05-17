#include "../../include/query/SimpleQueryParser.hpp"
#include <cctype>
#include <fstream>

using namespace Utils;

template <typename T>
void SimpleQueryParser::setQuery(const T &queryContent) {
  this->query.clear();
  this->query.str(std::string());
  this->query << queryContent;
}

SimpleQueryParser::SimpleQueryParser(const std::string &query) {
  setQuery(query);
}

SimpleQueryParser::SimpleQueryParser(const SimpleQueryParser &obj) {
  setQuery(obj.query.str());
}

void SimpleQueryParser::checkFile(const std::string &filePath,
                                  const std::string &fileName) {
  std::string fullFilePath = filePath + '/' + fileName;

  if (fileName.find(".sql") == std::string::npos) {
    fullFilePath += ".sql";
  }

  std::fstream file {fullFilePath};
  if (!file.is_open()) {
    throw std::runtime_error("(SimpleQueryParser): File ( " +  fullFilePath + " ) not found");
  } else {
    setQuery(file.rdbuf());
  }
}

SimpleQueryParser::VariablePos SimpleQueryParser::setPositionOfVaraibles(const std::string &fileContent,
                                                                         QueryParams &queryParams) const {
  VariablePos resultMap;
  size_t varStartPos = fileContent.find("$");
  size_t varEndPos;

  while (varStartPos < std::string::npos) {
    std::string varName;
    for (int i = varStartPos + 1; i < fileContent.length(); i++) {
      if (std::isalpha(fileContent[i]) || fileContent[i] == '_') {
        varName += fileContent[i];
      } else break;
    }

    if (queryParams.find(varName) != queryParams.end()) {
      varEndPos = varStartPos + varName.length();
      std::string tempSubStr = fileContent.substr(varEndPos + 1, fileContent.length() - varEndPos);

      resultMap[varName] = std::make_pair(varStartPos, varEndPos + 1);
      varStartPos = tempSubStr.find("$");
      if (varStartPos != std::string::npos) {
        varStartPos = fileContent.length() - (tempSubStr.length() - varStartPos);
      }
    } else {
      throw std::runtime_error("(SimpleQueryParser) : " + varName + " not found in the query file");
    }
  }
  return resultMap;
}

bool SimpleQueryParser::checkForNumber(const std::string &data) const {
  bool result {true};
  for (const auto &i : data) {
    if (!std::isdigit(i)) {
      result = false;
      break;
    }
  }
  return result;
}

SimpleQueryParser::DataType SimpleQueryParser::getDataType(const std::string &data) const {
  return (checkForNumber(data) ? DataType::NUMBER : DataType::STRING);
}

void SimpleQueryParser::modifyPositionOfVariables(const size_t fromPos,
                                                  VariablePos &variablePos,
                                                  const size_t changeFactor,
                                                  const PositionChangeType &operationType) {
  size_t change;
  switch (operationType) {
    case PositionChangeType::SUBTRACT:
      change = -(changeFactor);
      break;
    case PositionChangeType::ADD:
      change = changeFactor;
      break;
  }

  int i = 0;
  for (auto &key : variablePos) {
    if (i >= fromPos) {
      key.second.first += change;
      key.second.second += change;
    }
  }
}

void SimpleQueryParser::injectDataIntoFileContent(VariablePos &variablePos,
                                                  const std::string &fileContent,
                                                  QueryParams &queryParams) {
  std::string resultFileContent = fileContent;
  const size_t lengthOfFileContent = resultFileContent.length();

  int i = 1;
  for (const auto &key : queryParams) {
    if (key.first != "file") {
      if (variablePos.find(key.first) != variablePos.end()) {
        auto keyPair = variablePos[key.first];

        size_t firstValue = keyPair.first;
        size_t secondValue = keyPair.second - keyPair.first;
        std::string value = std::get<std::string>(key.second);
        if (getDataType(value) == DataType::STRING) {
          value = '"' + value + '"';
        }

        std::string varValue = resultFileContent.substr(firstValue, secondValue);
        resultFileContent.replace(firstValue, secondValue, value);

        size_t changeFactor;
        if (resultFileContent.length() < lengthOfFileContent) {
          changeFactor = lengthOfFileContent - resultFileContent.length();
          modifyPositionOfVariables(i, variablePos, changeFactor, PositionChangeType::SUBTRACT);
        } else if (resultFileContent.length() > lengthOfFileContent) {
          changeFactor = resultFileContent.length() - lengthOfFileContent;
          modifyPositionOfVariables(i, variablePos, changeFactor, PositionChangeType::ADD);
        }
      } else {
        throw std::runtime_error("(SimpleQueryParser) : Variable ( " + key.first + " ) not found, while injecting data");
      }
    }
    i++;
  }
  setQuery(resultFileContent);
}

void SimpleQueryParser::parseQueryHelper(const std::string &filePath,
                                                QueryParams &queryParams) {
  if (queryParams.find("file") != queryParams.end()) {
    checkFile(filePath, std::get<std::string>(queryParams["file"]));
    std::string fileContent = getParsedData();

    auto posOfVars = setPositionOfVaraibles(fileContent, queryParams);
    injectDataIntoFileContent(posOfVars, fileContent, queryParams);
  } else {
    throw std::runtime_error("(SimpleQueryParser) : file parameter not specified");
  }
}
