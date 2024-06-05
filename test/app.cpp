#include <crow.h>
#include <crow/app.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/mustache.h>
#include <httplib.h>

std::string getFieldValue(const std::string &body,
                          const std::string &fieldValue) {
  httplib::Request req {body};
  return req.get_header_value(fieldValue);
}

int main() {
  crow::SimpleApp app;
  /* CROW_ROUTE (app, "/<string>")([](std::string name){ */
  /*   auto page = crow::mustache::load("index.html"); */
  /*   crow::mustache::context ctx ({{"person", name}}); */
  /*   return page.render(ctx); */
  /* }); */

  CROW_ROUTE(app, "/")([](){
    auto page = crow::mustache::load("index.html");
    return page.render();
  });

  CROW_ROUTE(app, "/hello")([](){
    auto page = crow::mustache::load("index.html");
    return page.render();
  });

  CROW_ROUTE(app, "/logout")([](const crow::request &req, crow::response &res) {
    res.redirect("/");
    res.end();
  });

  CROW_ROUTE(app, "/submitForm").methods("POST"_method)
    ([](const crow::request &req) {
      std::cout << req.body << std::endl;
      std::cout << getFieldValue(req.body, "Content-Type") << std::endl;

      auto res = crow::response(302);
      res.set_header("Location", "/logout");
      return res;
    });

  app.port(8082).multithreaded().run();
  return 0;
}
