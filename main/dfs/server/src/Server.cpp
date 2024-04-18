#include "../include/Server.hpp"

Server::Server()
  : serverHandler(std::make_unique<ServerHandler>()) {}

void Server::start() {
  serverHandler->startListening();
}
