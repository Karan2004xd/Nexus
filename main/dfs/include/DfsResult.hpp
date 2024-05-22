#ifndef DFS_RESULT_HPP
#define DFS_RESULT_HPP

#include "Nexus.hpp"
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class Nexus::DfsResult {
public:
  typedef std::unordered_map<std::string, std::vector<std::string>> MessageMap;
  enum ResultType { SUCCESS, FAILED };
  DfsResult() {};
  DfsResult(const std::string &msg,
            const std::string &errorMsg,
            const ResultType &resultType);

  DfsResult(const MessageMap &messageMap,
            const std::string &errorMsg,
            const ResultType &resultType);

  const bool isSuccess() const { return resultType == ResultType::SUCCESS; }
  MessageMap getOutputList() const { return this->messageMap; }

  const std::string getOutput() const { return this->message.str(); }
  const std::string getErrorMsg() const { return this->errorMsg; }

private:
  ResultType resultType;
  std::string errorMsg;

  MessageMap messageMap;
  std::ostringstream message;

  void clearMessage();
  void setMessage(const std::string &msg);
};
#endif // DFS_RESULT_HPP
