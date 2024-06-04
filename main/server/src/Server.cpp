#include "../include/Server.hpp"

using namespace Nexus;

void Server::run() {
  ServerHandler::startListeningForRequests();
  ServerHandler::startLogging();
}
