#ifndef CONTENT_HPP
#define CONTENT_HPP

#include "Data.hpp"
#include <string>

class Data::Content {
public:
  Content() = delete;
  Content(const std::string &);
  Content(const std::string &, const std::string &);

  std::string getFileName() const { return this->fileName; }
  int getContentLength() const { return this->contentLength; }

  std::string getMainBody() const { return this->mainBody; }
  std::string getContentType() const { return this->contentType; }

private:
  std::string fileName;
  int contentLength;
  std::string mainBody;
  std::string contentType;

  void setFileName(const std::string &);
  void setFileMainBodyInfo(const std::string &);
  void setContentType(const std::string &);

  std::string findVariableValue(const std::string &, const std::string &, char);
  std::string findMainBody(const std::string &);

  void filterData(const std::string &);
};
#endif // CONTENT_HPP
