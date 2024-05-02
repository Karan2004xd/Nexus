#ifndef JSON_STRING_BUILDER_HPP
#define JSON_STRING_BUILDER_HPP

#include "Database.hpp"
#include <sstream>
#include <string>
#include <vector>

class Database::JsonStringBuilder {
public:
  JsonStringBuilder();

  JsonStringBuilder &singleData(const std::string &, const std::string &);
  JsonStringBuilder &listOfData(const std::string &, const std::vector<std::string> &);
  void build();

  std::string str() const { return jsonData.str(); }
private:
  std::ostringstream jsonData;
};
#endif // JSON_STRING_BUILDER_HPP
