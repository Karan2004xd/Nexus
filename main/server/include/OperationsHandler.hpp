#ifndef OPERATIONS_HANDLER_HPP
#define OPERATIONS_HANDLER_HPP
#include <variant>
#include "../../dfs/include/Dfs.hpp"

class OperationsHandler : protected Nexus::Dfs {
public:
  OperationsHandler() {};
  enum OperationType { USER, DFS };
  std::string getOperationResult(std::string &jsonData,
                                 const OperationType &operationType,
                                 const std::string &username = {});

  bool userOperationIsSuccess(std::string &encodedJson);

  typedef std::unordered_map<std::string, std::variant<std::string, std::vector<std::string>>> JsonDataParams;
private:
  DfsResult getUserOperationsResult(JsonDataParams &jsonParams);
  DfsResult getDfsOperationsResult(JsonDataParams &jsonParams,
                                   const std::string &username);

  std::string getOperationResultInJson(const DfsResult &result);
};
#endif // OPERATIONS_HANDLER_HPP
