#ifndef DFS_RESULT_HPP
#define DFS_RESULT_HPP

#include "Nexus.hpp"
#include <sstream>
#include <string>

class Nexus::DfsResult {
public:
  enum ResultType { SUCCESS, FAILED };
  DfsResult(const std::string &msg,
            const std::string &errorMsg,
            const ResultType &resultType);

  const bool isSuccess() const { return resultType == ResultType::SUCCESS; }
  const std::string getOutput() const { return this->message.str(); }
  const std::string getErrorMsg() const { return this->errorMsg; }

private:
  ResultType resultType;
  std::string errorMsg;
  std::ostringstream message;

  void clearMessage();
  void setMessage(const std::string &msg);
};
#endif // DFS_RESULT_HPP
