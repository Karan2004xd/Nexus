#ifndef DATA_HPP
#define DATA_HPP

/* ------WebKitFormBoundaryTbXFHVZBpFKPz18f */
/* Content-Disposition: form-data; name="uploaded_file"; filename="leetcode.cpp" */
/* Content-Type: text/x-c++src */

#include <string>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include "../../../constants.h"

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
