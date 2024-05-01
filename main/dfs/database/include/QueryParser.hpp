#ifndef QUERY_PARSER_HPP
#define QUERY_PARSER_HPP
#include <fstream>

class QueryParser {
public:
  void parseQueryFile(const std::string &);
private:
  std::fstream file;
  bool isFileOpen = false;
  std::string fileContent;

  void checkFile(const std::string &);

  bool checkForDataInSyntax();
  bool checkForCorrentVarCount();
};
#endif // QUERY_PARSER_HPP
