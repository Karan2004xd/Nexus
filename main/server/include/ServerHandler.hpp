#ifndef SERVER_HANDLER_HPP
#define SERVER_HANDLER_HPP

#include "Routes.hpp"
#include <crow/app.h>

class ServerHandler {
public:
  ServerHandler() {}
  void startServer();
private:
  crow::SimpleApp app;
  Routes routes;
};
#endif // SERVER_HANDLER_HPP

