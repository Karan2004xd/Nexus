#ifndef QUERY_PARSER_HPP
#define QUERY_PARSER_HPP

#include "JsonDataHandler.hpp"
#include <fstream>

class QueryParser : protected JsonDataHandler {
protected:
  void parseJsonData(std::string &);
  std::string getParsedQuery() { return this->fileContent; }

private:
  std::string encodedJsonData;
  decodedJson parsedJsonData;

  void readFile();
  void injectDataIntoFile();
  void parseQueryFile(const std::string &);

  std::fstream file;
  std::string fileContent;

  void checkFile(const std::string &);

  bool checkForDataInSyntax();
  bool checkForCorrentVarCount();
};
#endif // QUERY_PARSER_HPP
