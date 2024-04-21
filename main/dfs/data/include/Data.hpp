#ifndef DATA_HPP
#define DATA_HPP

#include <string>

class Data {
public:
  Data() = delete;
  Data(const std::string &);

  std::string getFileName() { return this->fileName; }
  int getContentLength() { return this->contentLength; }
  std::string getMainBody() { return this->mainBody; }
private:
  std::string fileName;
  int contentLength;
  std::string mainBody;

  void setFileName(const std::string &);
  void setFileMainBodyInfo(const std::string &);

  std::string findVariableValue(const std::string &, const std::string &);
  std::string findMainBody(const std::string &);
  void filterData(const std::string &);
};
#endif // DATA_HPP
