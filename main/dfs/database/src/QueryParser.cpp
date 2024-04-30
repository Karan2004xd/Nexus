#include "../include/QueryParser.hpp"
#include <sstream>

void QueryParser::checkFile(const std::string &filePath) {
  file.open(filePath);

  if (!file.is_open()) {
    throw std::runtime_error(filePath + " was not found");
  } else {
    isFileOpen = true;
  }
}

bool QueryParser::checkForDataInSyntax() { 
  size_t pos = fileContent.find("$");
  while (pos < std::string::npos) {
    if (fileContent[pos + 1] != '{' && fileContent[pos + 2] != '}') {
      return false;
    }
    pos += 3;
    pos += fileContent.substr(pos, fileContent.length() - pos).find("$");
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
