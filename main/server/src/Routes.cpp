#include "../include/Routes.hpp"
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <crow/mustache.h>
#include "../include/OperationException.hpp"

void Routes::setCurrentUsername(const std::string &username) {
  this->currentUsername = username;
}

const std::string Routes::getCurrentUsername() const {
  return this->currentUsername;
}

void Routes::mapRoutes(crow::SimpleApp &app) {
  // Home Page
  CROW_ROUTE(app, "/")([&](const crow::request &req){
    if (currentUsername.empty()) {
      auto res = crow::response(302);
      res.set_header("Location", "/login");
      return res;
    } else {
      auto page = crow::mustache::load("index.html");
      return crow::response(page.render());
    } 
  });
  
  // Login Page
  CROW_ROUTE(app, "/login")([](){
    auto page = crow::mustache::load("login.html");
    return page.render();
  });

  // SignUp Page
  CROW_ROUTE(app, "/sign-up")([](){
    auto page = crow::mustache::load("sign-up.html");
    return page.render();
  });

  // User Data Handling Endpoint
  CROW_ROUTE(app, "/user").methods(crow::HTTPMethod::POST)
    ([&](const crow::request &request){
      std::string requestBody = request.body;
      crow::response response;
      try {
        std::string responseBody = operations.getOperationResult(requestBody,
                                                                 OperationsHandler::OperationType::USER);
        if (operations.userOperationIsSuccess(responseBody)) {
          auto tempJsonData = crow::json::load(requestBody);
          std::string username = tempJsonData["username"].s();
          setCurrentUsername(username);

          response = crow::response(200, responseBody);
        } else {
          response = crow::response(409, "Unable to perform the desired operation");
        }
      } catch (const OperationException &e) {
        std::cout << e.what() << std::endl;
        response = crow::response(400, "Bad Request");
      } catch (const std::exception &) {
        response = crow::response(500, "Internal Error, some exception occured");
      }
      return response;
    });

  // DFS Operations Endpoint
  CROW_ROUTE(app, "/dfs-operations").methods(crow::HTTPMethod::POST)
    ([&](const crow::request &request){
      std::string requestBody = request.body;
      crow::response response;
      try {
        std::string responseBody = operations.getOperationResult(requestBody,
                                                                 OperationsHandler::OperationType::DFS,
                                                                 currentUsername);
        response = crow::response(200, responseBody);
      } catch (const OperationException &e) {
        std::cout << e.what() << std::endl;
        response = crow::response(400, "Bad Request");
      } catch (const std::exception &) {
        response = crow::response(500, "Internal Error, some exception occured");
      }
      return response;
    });
}
