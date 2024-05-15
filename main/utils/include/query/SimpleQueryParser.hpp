#ifndef SIMPLE_QUERY_PARSER_HPP
#define SIMPLE_QUERY_PARSER_HPP
#include "../../../dfs/include/Nexus.hpp"
#include "QueryParser.hpp"
#include <sstream>

using namespace Nexus;

class Utils::SimpleQueryParser : protected QueryParser {
public:
  static SimpleQueryParser parseQuery(const std::string &filePath,
                                QueryParams &queryParams,
                                SimpleQueryParser *parser = new SimpleQueryParser()) {
    parser->parseQueryHelper(filePath, queryParams);
    return {};
  }

  std::string getParsedData() const override {
    return this->query.str();
  }

protected:
  SimpleQueryParser() {}
  void parseQueryHelper(const std::string &filePath,
                               QueryParams &queryParams) override;

private:
  std::ostringstream query;
  typedef std::unordered_map<std::string, std::pair<size_t, size_t>> VariablePos;
  VariablePos positionOfVariables;

  void checkFile(const std::string &filePath,
                 const std::string &fileName);

  size_t checkForSquareBrackets(const std::string &fileContent,
                              const size_t startPos,
                              const size_t endPos,
                              const char identifier) const;

  std::pair<size_t, size_t> checkForCorrectVarSyntax(const std::string &fileContent,
                                const std::string &varName) const;

  void setPositionOfVaraibles(const QueryParams &queryParams);

  template <typename T>
  void setQuery(const T &queryContent);
  void injectDataIntoFileContent();
};
#endif // SIMPLE_QUERY_PARSER_HPP
