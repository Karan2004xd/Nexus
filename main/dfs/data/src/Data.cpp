#include "../include/Data.hpp"
#include "../../../constants.h"
#include <boost/algorithm/string/trim.hpp>
#include <iostream>

void Data::setFileName(const std::string &fileName) {
  if (!fileName.empty()) {
    this->fileName = fileName;
  }
}

void Data::setContentType(const std::string &contentType) {
  if (!contentType.empty()) {
    this->contentType = contentType;
  }
}

void Data::setFileMainBodyInfo(const std::string &mainBody) {
  if (!mainBody.empty()) {
    this->mainBody = mainBody;
    this->contentLength = mainBody.length();
  }
}

std::string Data::findVariableValue(const std::string &variableName, const std::string &rawData) {
  size_t indexOfVar = rawData.find(variableName);
  std::string resultValue = {};

  if (indexOfVar != std::string::npos) {
    int resultIndex = indexOfVar + variableName.length() + 2;
    while (rawData[resultIndex] != '"') {
      resultValue += rawData[resultIndex];
      resultIndex++;
    }
  }
  return resultValue;
}

std::string Data::findMainBody(const std::string &rawData) {
  size_t contentTypePos = rawData.find(std::string(CONTENT_TYPE));
  std::string resultValue = {};
  
  if (contentTypePos != std::string::npos) {
    std::string tempOuterMainBody = rawData.substr(contentTypePos, rawData.length());
    size_t mainBodyStartPos = tempOuterMainBody.find("\r");
    size_t mainBodyEndPos = tempOuterMainBody.find(std::string(FORM_BOUNDRY));

    if (mainBodyStartPos != std::string::npos &&
        mainBodyEndPos != std::string::npos) {
      resultValue = tempOuterMainBody.substr(mainBodyStartPos, mainBodyEndPos - mainBodyStartPos);
    }
  }
  boost::algorithm::trim(resultValue);
  return resultValue;
}

void Data::filterData(const std::string &rawData) {
  std::string tempFileName = findVariableValue(std::string(FILENAME), rawData);
  /* std::string tempContentType = findVariableValue(std::string(CONTENT_TYPE), rawData); */
  std::string tempMainBody = findMainBody(rawData);

  setFileName(tempFileName);
  /* setContentType(tempContentType); */
  setFileMainBodyInfo(tempMainBody);
}

Data::Data(const std::string &rawData) {
  std::cout << rawData << std::endl;
  filterData(rawData);
  std::cout << "FileName: " << getFileName()
            << "\nContent-Type: " << getContentType()
            << "\nMain Body: \n" << getMainBody()
            << std::endl;
  DataChunker::chunkData(getMainBody(), getContentLength(), getContentType());
}
