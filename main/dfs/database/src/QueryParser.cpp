#include "../include/QueryParser.hpp"
#include "../../../constants.h"

void QueryParser::checkFile(const std::string &filePath) {
  if (file.is_open()) {
    file.close();
  }
  file.open(std::string(QUERIES_DIR) + '/' + filePath + ".sql");

  if (!file.is_open()) {
    throw std::runtime_error(filePath + " was not found");
  } 
}

bool QueryParser::checkForDataInSyntax() {
  size_t pos = fileContent.find('$');

  while (pos < std::string::npos) {
    if (fileContent[pos + 1] == '{') {

      std::string param;
      for (int i = pos + 2; i < fileContent.length(); i++) {
        if (std::isalpha(fileContent[i]) != 0 || fileContent[i] == '_') {
          param += fileContent[i];
        } else if (fileContent[i] == '}') {
          break;
        } else {
          return false;
        }
      }
      
      if (parsedJsonData.find(param) != parsedJsonData.end()) {
        size_t currentPos = pos + param.length() + 3;

        std::string tempFileContent = fileContent.substr(currentPos, fileContent.length() - currentPos);
        pos = tempFileContent.find('$');

        if (pos != std::string::npos) {
          pos = fileContent.length() - (tempFileContent.length() - pos);
        }
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

void QueryParser::injectDataIntoFile() {
  size_t pos = fileContent.find('$');
  std::string firstHalfContent = fileContent.substr(0, pos);
  std::string secondHalfContent = fileContent.substr(pos + 1, fileContent.length() - pos);

  fileContent = firstHalfContent;
  for (int i = 0; i < secondHalfContent.length(); i++) {
    if (secondHalfContent[i] == '{') {
      std::string param;
      i++;

      while (secondHalfContent[i] != '}') {
        param += secondHalfContent[i];
        i++;
      }
      fileContent += parsedJsonData[param];
    } else if (secondHalfContent[i] != '$') {
      fileContent += secondHalfContent[i];
    }
  }
}

void QueryParser::readFile() {
  std::string line;
  fileContent.clear();

  while (std::getline(file, line)) {
    if (line.find("--") == std::string::npos) {
      fileContent += line;
    }
  }
}

void QueryParser::parseQueryFile(const std::string &fileName) {
  checkFile(fileName);
  readFile();

  if (parsedJsonData.size() > 1) {
    if (checkForDataInSyntax()) {
      injectDataIntoFile();

    } else throw std::runtime_error("Incorrect data injection \nQuery : " + fileContent);
  }
}

void QueryParser::parseJsonData(std::string &jsonData) {
  if (jsonData.find("file") != std::string::npos) {
    encodedJsonData = JsonDataHandler::encodeStringToJson(jsonData);
    parsedJsonData = JsonDataHandler::decodeJsonToString(encodedJsonData);

    parseQueryFile(parsedJsonData["file"]);
  } else {
    throw std::runtime_error("QueryParser : 'file' parameter was not found in the json ( " + jsonData + " ) file");
  }
}
