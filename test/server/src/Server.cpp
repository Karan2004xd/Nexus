#include "../include/Server.h"

Server::Server(int port, const std::string &ip_address) {
  this->serverHandler = std::make_unique<ServerHandler>(port, ip_address);
}

Server::Server(int port) {
  this->serverHandler = std::make_unique<ServerHandler>(port, IP_ADDRESS);
}

Server::Server() {
  this->serverHandler = std::make_unique<ServerHandler>(PORT, IP_ADDRESS);
}

void Server::start_server() {
  this->serverHandler->start_data_listener();
}
