#include "../../include/query/SimpleQueryParser.hpp"
#include <cctype>
#include <fstream>
#include <iostream>

using namespace Utils;

template <typename T>
void SimpleQueryParser::setQuery(const T &queryContent) {
  query.clear();
  query.str(std::string());
  query << queryContent;
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

size_t SimpleQueryParser::checkForSquareBrackets(const std::string &fileContent,
                                                 const size_t startPos,
                                                 const size_t endPos,
                                                 const char identifier) const {
  for (int i = startPos; i < endPos; i++) {
    if (fileContent[i] == identifier) {
      return i;
    } else if (fileContent[i] != ' ') break;
  }
  return std::string::npos;
}

std::pair<size_t, size_t> SimpleQueryParser::checkForCorrectVarSyntax(const std::string &fileContent,
                                                                      const std::string &varName) const {
  std::pair<size_t, size_t> resultPair;
  std::cout << fileContent << std::endl;
  size_t varStartPos = fileContent.find(varName);

  if (varStartPos != std::string::npos) {
    size_t varEndPos = varStartPos + varName.length();
    size_t startBracketPos = checkForSquareBrackets(fileContent, 0, varStartPos, '[');
    size_t endBracketPos = checkForSquareBrackets(fileContent, varEndPos, fileContent.length(), ']');

    if (startBracketPos == std::string::npos || endBracketPos == std::string::npos) {
      checkForCorrectVarSyntax(fileContent.substr(varEndPos, fileContent.length() - varEndPos), 
                               varName);
    }

    resultPair = std::make_pair(varStartPos - startBracketPos, endBracketPos + varEndPos);
  } else {
    throw std::runtime_error("(SimpleQueryParser) : " + varName + " not found in the query file");
  }
  return resultPair;
}

void SimpleQueryParser::setPositionOfVaraibles(const QueryParams &queryParams) {
  std::string fileContent = query.str();
  for (const auto &key : queryParams) {
    auto pairOfPos = checkForCorrectVarSyntax(fileContent, key.first);
    std::string vars = fileContent.substr(pairOfPos.first, pairOfPos.second - pairOfPos.first);
    std::cout << vars << std::endl;
  }
}

void injectDataIntoFileContent();

void SimpleQueryParser::parseQueryHelper(const std::string &filePath,
                                                QueryParams &queryParams) {
  if (queryParams.find("file") != queryParams.end()) {
    checkFile(filePath, std::get<std::string>(queryParams["file"]));
    setPositionOfVaraibles(queryParams);
  } else {
    throw std::runtime_error("(SimpleQueryParser) : file parameter not specified");
  }
}
