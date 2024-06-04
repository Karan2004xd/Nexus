#ifndef SIMPLE_QUERY_PARSER_HPP
#define SIMPLE_QUERY_PARSER_HPP
#include "../../../dfs/include/Nexus.hpp"
#include "QueryParser.hpp"
#include <map>
#include <sstream>

using namespace Nexus;

class Utils::SimpleQueryParser : protected QueryParser {
public:
  static SimpleQueryParser parseQuery(const std::string &filePath,
                                      QueryParams &queryParams,
                                      SimpleQueryParser *parser = new SimpleQueryParser()) {
    parser->parseQueryHelper(filePath, queryParams);
    SimpleQueryParser parserResult {parser->getParsedData()};
    return {parserResult};
  }

  const std::string getParsedData() const override {
    return this->query.str();
  }

  typedef QueryParams QueryParams;

protected:
  SimpleQueryParser() {}
  SimpleQueryParser(const SimpleQueryParser &obj);
  SimpleQueryParser(const std::string &query);

  void parseQueryHelper(const std::string &filePath,
                        QueryParams &queryParams) override;

private:
  std::ostringstream query;
  typedef std::map<std::string, std::pair<size_t, size_t>> VariablePos;

  void checkFile(const std::string &filePath,
                 const std::string &fileName);

  VariablePos setPositionOfVaraibles(const std::string &fileContent,
                                     QueryParams &queryParams) const;

  template <typename T>
  void setQuery(const T &queryContent);

  enum class DataType { NUMBER, STRING };
  bool checkForNumber(const std::string &data) const;
  DataType getDataType(const std::string &data) const;

  enum class PositionChangeType { ADD, SUBTRACT };

  void modifyPositionOfVariables(const size_t fromPos,
                                 VariablePos &variablePos,
                                 const size_t changeFactor,
                                 const PositionChangeType &operationType);

  void injectDataIntoFileContent(VariablePos &variablePos,
                                 const std::string &fileContent,
                                 QueryParams &queryParams);
};
#endif // SIMPLE_QUERY_PARSER_HPP
