#include "../include/Content.hpp"
#include <boost/algorithm/string/trim.hpp>

using namespace Data;

void Content::setFileName(const std::string &fileName) {
  if (!fileName.empty()) {
    this->fileName = fileName;
  }
}

void Content::setContentType(const std::string &contentType) {
  if (!contentType.empty()) {
    this->contentType = contentType;
  }
}

void Content::setFileMainBodyInfo(const std::string &mainBody) {
  if (!mainBody.empty()) {
    this->mainBody = mainBody;
    this->contentLength = mainBody.length();
  }
}

std::string Content::findVariableValue(const std::string &variableName, 
                                    const std::string &rawData,
                                    char delimeter = '"') {
  size_t indexOfVar = rawData.find(variableName);
  std::string resultValue = {};

  if (indexOfVar != std::string::npos) {
    int resultIndex = indexOfVar + variableName.length() + 2;
    while (rawData[resultIndex] != delimeter) {
      resultValue += rawData[resultIndex];
      resultIndex++;
    }
  }
  return resultValue;
}

std::string Content::findMainBody(const std::string &rawData) {
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

void Content::filterData(const std::string &rawData) {
  std::string tempFileName = findVariableValue(std::string(FILENAME), rawData);
  std::string tempContentType = findVariableValue(std::string(CONTENT_TYPE), rawData, '\r');
  std::string tempMainBody = findMainBody(rawData);

  setFileName(tempFileName);
  setContentType(tempContentType);
  setFileMainBodyInfo(tempMainBody);
}

Content::Content(const std::string &rawData) {
  filterData(rawData);
}
