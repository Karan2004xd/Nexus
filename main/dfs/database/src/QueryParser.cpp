#include "../include/QueryParser.hpp"
#include <algorithm>
#include <sstream>

void QueryParser::checkFile(const std::string &filePath) {
  file.open(filePath);

  if (!file.is_open()) {
    throw std::runtime_error(filePath + " was not found");
  } else {
    isFileOpen = true;
  }
}

bool QueryParser::checkForCorrentVarCount() {
  int numOfOpening = std::count(fileContent.begin(), fileContent.end(), '{');
  int numOfClosing = std::count(fileContent.begin(), fileContent.end(), '}');

  return numOfOpening == numOfClosing;
}

bool QueryParser::checkForDataInSyntax() { 
  if (checkForCorrentVarCount()) {
    size_t pos = fileContent.find("$");
    if (fileContent[pos + 1] == '{') {
      for (int i = pos + 2; i < fileContent.length(); i++) {
        if (fileContent[i] == ' ' || fileContent[i] == ',') {
          return false;
        } else if (fileContent[i] == '}') {
          // start working from here
        }
      }
    }
  } else {
    return false;
  }
  return true;
}

void QueryParser::parseQueryFile(const std::string &fileName) {
  if (!isFileOpen) checkFile(fileName);
  std::ostringstream oss;
  oss << file.rdbuf();

  fileContent = oss.str();
  if (fileContent.find("$") != std::string::npos) {
    if (checkForDataInSyntax()) {
      // start working from here
      
    } else throw std::runtime_error("Incorrect data injection syntax");
  }
}
