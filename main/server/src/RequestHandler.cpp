#include "../include/RequestHandler.hpp"
#include "../../constants.h"
#include "../include/Logging.hpp"
#include <fstream>
#include <iterator>
#include <iostream>

std::string RequestHandler::getRequestTarget(const std::string &requestTarget) {
  std::string target;
  if (requestTarget.empty() || requestTarget[0] != '/') {
    target = "/";
  } else {
    target = std::string(requestTarget.data(), requestTarget.size());
  }
  return target;
}

bool RequestHandler::endsWith(const std::string &str, const std::string &suffix) {
  if (suffix.length() > str.length()) return false;
  return str.substr(str.length() - suffix.length()) == suffix;
}

std::string RequestHandler::getStaticFileContent(const std::string &fileName) {
  std::string path = std::string(PATH_TO_INTERFACES) + fileName;

  std::string fileContent;
  std::ifstream file {path, std::ios::binary};

  if (!file.is_open()) {
    BOOST_LOG_TRIVIAL(warning) << "File at path " << path << " was not found";
    throw std::runtime_error("File Not found");
  } else {
    BOOST_LOG_TRIVIAL(info) << "Loading static data from : " << path;
    fileContent = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  }
  return fileContent;
}

std::string RequestHandler::getMimeType(const std::string &arg) {
  if (endsWith(arg, ".html")) return "text/html";
  if (endsWith(arg, ".css")) return "text/css";
  if (endsWith(arg, ".js")) return "text/javascript";
  return "application/octet_stream"; // Default MIME Type
}

std::string RequestHandler::getContentType(const http::request<http::string_body> &request) {
  auto contentTypeIt = request.find(http::field::content_type);
  std::string contentType = (contentTypeIt != request.end()) ? contentTypeIt->value() : "";
  return contentType;
}

bool RequestHandler::checkJsonData(const std::initializer_list<std::string> &jsonArgs,
                                   const JsonDataParams &jsonParams) {
  for (const auto &arg : jsonArgs) {
    if (jsonParams.find(arg) == jsonParams.end()) return false;
  }
  return true;
}

DfsResult RequestHandler::getOperationResult(const JsonDataParams &jsonParams) {
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
  }

  return resultObj;
}

RequestHandler::RequestMap RequestHandler::getRequestMap(const JsonDataParams &jsonParams,
                                                         const std::string &contentType,
                                                         const unsigned int &version) {
  RequestMap result {
    {RequestParams::CONTENT_TYPE, contentType},
    {RequestParams::VERSION, version}
  };

  auto resultObj = getOperationResult(jsonParams);
  if (resultObj.isSuccess()) {
    if (resultObj.getOutput().empty()) {
      result.insert({RequestParams::BODY, resultObj.getOutputList()});
    } else {
      result.insert({RequestParams::BODY, resultObj.getOutput()});
    }
    result.insert({RequestParams::STATUS, StatusCode::ok});
  } else {
    result.insert({RequestParams::BODY, resultObj.getErrorMsg()});
    result.insert({RequestParams::STATUS, StatusCode::bad_request});
  }
  return result;
}

std::string RequestHandler::getResponseData(const RequestMap &requestMap) {
  std::string body;
  try {
    body = std::get<std::string>(requestMap.at(RequestParams::BODY));
  } catch (const std::exception &) {
    auto jsonData = Utils::SimpleJsonParser::JsonBuilder();
    auto tempBody = std::get<std::unordered_map<std::string, std::vector<std::string>>>(
      requestMap.at(RequestParams::BODY)
    );

    for (const auto &key : tempBody) {
      jsonData = jsonData.multipleData(key.first, key.second);
    }
    auto jsonOutput = jsonData.getJsonData();
    body = Utils::SimpleJsonParser::encodeJson(jsonOutput);
  }
  std::string contentType = std::get<std::string>(requestMap.at(RequestParams::CONTENT_TYPE));
  std::string status;

  StatusCode tempStatus = std::get<StatusCode>(requestMap.at(RequestParams::STATUS));
  if (tempStatus == StatusCode::ok) {
    status = "ok";
  } else if (tempStatus == StatusCode::bad_request) {
    status = "bad_request";
  } else if (tempStatus == StatusCode::not_found) {
    status = "not_found";
  }

  std::string version = std::to_string(
    std::get<unsigned int>(requestMap.at(RequestParams::VERSION))
  );

  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("body", body)
    .singleData("content-type", contentType)
    .singleData("status", status)
    .singleData("version", version)
    .getJsonData();

  return Utils::SimpleJsonParser::encodeJson(jsonData);
}

std::string RequestHandler::getResponseData(const std::string &target,
                                            const unsigned int &version) {
  RequestMap result;
  std::string body, contentType, fileName;

  if (target == "/") {
    fileName = "/index.html";
  } else {
    fileName = target;
  }
  StatusCode status;

  try {
    contentType = getMimeType(fileName);
    body = getStaticFileContent(fileName);
    status = StatusCode::ok;
  } catch (const std::exception &) {
    status = StatusCode::not_found;
  }

  result = {
    {RequestParams::BODY, body},
    {RequestParams::VERSION, version},
    {RequestParams::CONTENT_TYPE, contentType},
    {RequestParams::STATUS, status}
  };

  return getResponseData(result);
}

std::string RequestHandler::handleRequestHelper(const http::request<http::string_body> &request) {
  std::string response;
  std::string requestTarget = getRequestTarget(request.target());
  std::string body = request.body();
  unsigned int version = request.version();

  if (request.method() == boost::beast::http::verb::post) {
    std::string contentType = getContentType(request);

    if (contentType == "application/json") {
      auto jsonData = Utils::SimpleJsonParser::decodeJson(body);
      if (checkJsonData({OPERATION}, jsonData)) {
        response = getResponseData(getRequestMap(jsonData, contentType, version));
      }
    }
  } else {
    response = getResponseData(requestTarget, version);
  }
  return response;
}
