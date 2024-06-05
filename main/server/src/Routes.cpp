#include "../include/Routes.hpp"
#include <crow/common.h>
#include <crow/http_request.h>

void Routes::setCurrentUsername(const std::string &username) {
  this->currentUsername = username;
}

const std::string Routes::getCurrentUsername() const {
  return this->currentUsername;
}

void Routes::mapRoutes(crow::SimpleApp &app) {
  // Home Page
  CROW_ROUTE(app, "/")([](){
    auto page = crow::mustache::load("index.html");
    return page.render();
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
}
