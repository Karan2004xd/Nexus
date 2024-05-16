#ifndef CONTENTS_HPP
#define CONTENTS_HPP

#include "../../dfs/include/Nexus.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"
#include <string>

using namespace Nexus;

class Data::Contents {
public:
  Contents() = delete;
  Contents(const Utils::SimpleJsonParser::JsonDataParams &jsonDataParams);

  const std::string getFileName() const { return this->fileName; }
  const std::string getFileType() const { return this->fileType; }
  const std::string getFileContent() const { return this->fileContent; }

  const size_t getfileLength() const { return this->fileLength; }
  const size_t getfileMemorySize() const { return this->fileMemorySize; }

private:
  std::string fileName;
  std::string fileType;

  size_t fileLength;
  size_t fileMemorySize;

  std::string fileContent;

  void checkStringData(const std::string &stringData);

  void setFileName(const std::string &fileName);
  void setFileContent(const std::string &fileContent);

  void setFileType();
  void setFileLength();
  void setFileMemorySize();

  void setFileDetails(const Utils::SimpleJsonParser::JsonDataParams &jsonDataParams);
};
#endif // CONTENTS_HPP
