#include "../include/Producer.hpp"
#include <cppkafka/configuration.h>
#include <cppkafka/producer.h>
#include "../include/Logging.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"

Producer::Producer() {
  this->producerConfig = {
    {"bootstrap.servers", KAFKA_BROKER},
    {"enable.idempotence", true}
  };
}

DfsResult Producer::getOperationResult(RequestMap &jsonParams) {
  std::string operation = std::get<std::string>(jsonParams.at(OPERATION));
  std::string username = std::get<std::string>(jsonParams.at(CLIENT_USERNAME));

  std::string fileName, password;
  try {
    fileName = std::get<std::string>(jsonParams.at(FILE_NAME));
  } catch (const std::exception &) {}

  try {
    password = std::get<std::string>(jsonParams.at(CLIENT_PASSWORD));
  } catch (const std::exception &) {}

  BOOST_LOG_TRIVIAL(info) << "Performing operation: " << operation;

  DfsResult resultObj;
  if (operation == PUT_DATA) {
    std::string fileContent = std::get<std::string>(jsonParams.at(FILE_CONTENT));
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
  } else if (operation == ADD_USER) {
    resultObj = Dfs::addUser(username, password);
  } else if (operation == CHECK_USER) {
    resultObj = Dfs::checkUser(username, password);
  } else {
    resultObj = DfsResult("", "Invalid operation request", DfsResult::ResultType::FAILED);
  }
  return resultObj;
}

std::string Producer::getJsonResposne(const std::string &requestData) {
  std::string tempRequestData = requestData;
  auto jsonData = Utils::SimpleJsonParser::decodeJson(tempRequestData);

  DfsResult result = getOperationResult(jsonData);
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
    resultJsonData.singleData("output", result.getErrorMsg());
  }
  auto finalResult = resultJsonData.getJsonData();
  return Utils::SimpleJsonParser::encodeJson(finalResult);
}

void Producer::sendResponse(const std::string &requestData) {
  try {
    cppkafka::Producer producer {producerConfig};
    std::string responseData = getJsonResposne(requestData);
    producer.produce(cppkafka::MessageBuilder(KAFKA_NEXUS_FRONTEND_TOPIC).partition(0).payload(responseData));
    BOOST_LOG_TRIVIAL(info) << "Data sent";
    producer.flush();
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
