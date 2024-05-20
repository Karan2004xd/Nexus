#ifndef QUERY_PARSER_HPP
#define QUERY_PARSER_HPP

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class QueryParser {
public:
  virtual ~QueryParser() {}
protected:
  QueryParser() {}
  typedef std::unordered_map<std::string, std::variant<std::string, std::vector<std::string>>> QueryParams;
  virtual void parseQueryHelper(const std::string &filePath, 
                                QueryParams &queryParams) = 0;
  virtual const std::string getParsedData() const = 0;
};

#endif // QUERY_PARSER_HPP
