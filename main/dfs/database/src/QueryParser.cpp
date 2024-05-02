#include "../include/QueryParser.hpp"
#include <iostream>
#include <sstream>

void QueryParser::checkFile(const std::string &filePath) {
  file.open(filePath);

  if (!file.is_open()) {
    throw std::runtime_error(filePath + " was not found");
  } else {
    isFileOpen = true;
  }
}

/* bool QueryParser::checkForCorrentVarCount() { */
/*   int numOfOpening = std::count(fileContent.begin(), fileContent.end(), '{'); */
/*   int numOfClosing = std::count(fileContent.begin(), fileContent.end(), '}'); */

/*   return numOfOpening == numOfClosing; */
/* } */

/* bool QueryParser::checkForDataInSyntax() { */ 
/*   if (checkForCorrentVarCount()) { */
/*     size_t pos = fileContent.find("$"); */
/*     while (pos < std::string::npos) { */
/*       if (fileContent[pos + 1] == '{') { */
/*         for (int i = pos + 2; i < fileContent.length(); i++) { */
/*           if (fileContent[i] == ' ' || fileContent[i] == ',') { */
/*             return false; */
/*           } else if (fileContent[i] == '}') { */
/*             pos = fileContent.substr(i + 1, fileContent.length() - i - 1).find('$'); */
/*             break; */
/*           } */
/*         } */
/*       } */
/*     } */
/*   } else { */
/*     return false; */
/*   } */
/*   return true; */
/* } */

bool QueryParser::checkForDataInSyntax() {
  for (const auto &key : parsedJsonData) {
    if (key.first != "file") {
      size_t pos = fileContent.find(key.first);
      std::cout << pos << std::endl;

      if (pos != std::string::npos &&
          fileContent[pos - 1] == '{' &&
          fileContent[pos + key.first.length()] == '}' &&
          fileContent[pos - 2] == '$') {
        continue;
      } else {
        return false;
      }
    }
  }
  return true;
}

void QueryParser::parseQueryFile(const std::string &fileName) {
  if (!isFileOpen) checkFile(fileName);
  std::ostringstream oss;
  oss << file.rdbuf();

  fileContent = oss.str();
  if (parsedJsonData.size() > 1) {
    if (checkForDataInSyntax()) {
      std::cout << "That works" << std::endl;
      // start working from here
      
    } else throw std::runtime_error("Incorrect data injection syntax");
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
