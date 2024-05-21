#include "../include/DfsResult.hpp"

using namespace Nexus;

DfsResult::DfsResult(const std::string &msg,
                     const std::string &errorMsg,
                     const ResultType &resultType) {
  this->resultType = resultType;
  this->errorMsg = errorMsg;
  setMessage(msg);
}

void DfsResult::clearMessage() {
  message.clear();
  message.str({});
}

void DfsResult::setMessage(const std::string &msg) {
  if (!message.str().empty()) {
    clearMessage();
  }
  message << msg;
}
