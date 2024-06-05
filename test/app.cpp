#include <crow.h>
#include <crow/app.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/multipart.h>
#include <crow/mustache.h>

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

  CROW_ROUTE(app, "/submitForm").methods(crow::HTTPMethod::POST)
    ([](const crow::request &req) {
      /* std::cout << req.body << std::endl; */

      /* std::cout << req.url_params.get("username") << std::endl; */
      /* std::cout << req.url_params.get("password") << std::endl; */

      crow::multipart::message msg {req};
      auto result = msg.parts;
      std::cout << result.size() << std::endl;

      for (const auto &i : result) {
        std::cout << i.body << std::endl;
      }

      auto res = crow::response(302);
      res.set_header("Location", "/logout");
      return res;
    });

  app.port(8082).multithreaded().run();
  return 0;
}
