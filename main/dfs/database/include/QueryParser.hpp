#ifndef QUERY_PARSER_HPP
#define QUERY_PARSER_HPP

#include "JsonDataHandler.hpp"
#include <fstream>

class QueryParser : protected JsonDataHandler {
public:
  void parseJsonData(std::string &);

private:
  std::string encodedJsonData;
  decodedJson parsedJsonData;

  void setData(decodedJson &);
  void parseQueryFile(const std::string &);

  std::fstream file;
  bool isFileOpen = false;
  std::string fileContent;

  void checkFile(const std::string &);

  bool checkForDataInSyntax();
  bool checkForCorrentVarCount();
};
#endif // QUERY_PARSER_HPP
