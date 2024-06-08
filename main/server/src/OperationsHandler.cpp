#include "../include/OperationsHandler.hpp"
#include "../../constants.h"
#include "../include/OperationException.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"

DfsResult OperationsHandler::getUserOperationsResult(JsonDataParams &jsonParams) {
  DfsResult resultObj;
  try {
    std::string operation = std::get<std::string>(jsonParams.at(OPERATION));
    std::string username = std::get<std::string>(jsonParams.at(CLIENT_USERNAME));
    std::string password = std::get<std::string>(jsonParams.at(CLIENT_PASSWORD));


    if (operation == ADD_USER) {
      resultObj = Dfs::addUser(username, password);
    } else if (operation == CHECK_USER) {
      resultObj = Dfs::checkUser(username, password);
    } else {
      throw OperationException("Operation not defined");
    }
  } catch (const std::exception &e) {
    throw OperationException(e.what());
  }
  return resultObj;
}

DfsResult OperationsHandler::getDfsOperationsResult(JsonDataParams &jsonParams,
                                                    const std::string &username) {
  DfsResult resultObj;
  try {
    std::string operation = std::get<std::string>(jsonParams.at(OPERATION));
    std::string fileContent, fileName;

    try {
      fileContent = std::get<std::string>(jsonParams.at(FILE_CONTENT));
    } catch (const std::exception) {}

    try {
      fileName = std::get<std::string>(jsonParams.at(FILE_NAME));
    } catch (const std::exception) {}

    if (operation == PUT_DATA) {
      resultObj = Dfs::storeDataApi(fileName, fileContent, username);
    } else if (operation == GET_DATA) {
      resultObj = Dfs::getDataApi(fileName, username);
    } else if (operation == GET_TRASH_DATA) {
      resultObj = Dfs::getTrashDataApi(fileName, username);
    } else if (operation == DELETE_DATA) {
      resultObj = Dfs::deleteDataApi(fileName, username);
    } else if (operation == DELETE_TRASH_DATA) {
      resultObj = Dfs::deleteTrashDataApi(fileName, username);
    } else if (operation == LIST_DATA) {
      resultObj = Dfs::listDataApi(username);
    } else if (operation == LIST_TRASH_DATA) {
      resultObj = Dfs::listTrashDataApi(username);
    } else {
      throw OperationException("Operation not defined");
    }
  } catch (const std::exception &e) {
    throw OperationException(e.what());
  }
  return resultObj;
}

std::string OperationsHandler::getOperationResultInJson(const DfsResult &result) {
  auto resultJsonData = Utils::SimpleJsonParser::JsonBuilder();
  if (result.isSuccess()) {
    if (result.getOutput().empty()) {
      auto outputList = result.getOutputList();
      for (const auto &list : outputList) {
        resultJsonData.multipleData(list.first, list.second);
      }
    } else {
      resultJsonData.singleData("output", result.getOutput());
    }
  } else {
    throw OperationException(result.getErrorMsg());
  }
  auto finalResult = resultJsonData.getJsonData();
  return Utils::SimpleJsonParser::encodeJson(finalResult);
}

std::string OperationsHandler::getOperationResult(std::string &jsonData,
                                                  const OperationType &operationType,
                                                  const std::string &username) {
  auto decodedJsonData = Utils::SimpleJsonParser::decodeJson(jsonData);
  DfsResult resultObj;

  if (operationType == OperationType::DFS) {
    resultObj = getDfsOperationsResult(decodedJsonData, username);
  } else if (operationType == OperationType::USER) {
    resultObj = getUserOperationsResult(decodedJsonData);
  }

  return getOperationResultInJson(resultObj);
}

bool OperationsHandler::userOperationIsSuccess(std::string &encodedJson) {
  auto jsonData = Utils::SimpleJsonParser::decodeJson(encodedJson);
  std::string output = std::get<std::string>(jsonData.at("output"));
  if (output == "true") return true;
  return false;
}
