#include "../include/ServerHandler.hpp"
#include "../include/Logging.hpp"
#include "../include/ConnectionHandler.hpp"

void ServerHandler::startListeningForRequests() {
  ConnectionHandler::startListener();
}

void ServerHandler::startLogging() {
  Logging::init();
}
