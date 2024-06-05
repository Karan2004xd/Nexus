#include "../include/ServerHandler.hpp"
#include "../../constants.h"

void ServerHandler::startServer() {
  routes.mapRoutes(app);
  app.port(DEFAULT_PORT);
}
