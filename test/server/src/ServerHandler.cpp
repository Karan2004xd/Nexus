#include "../include/ServerHandler.h"

ServerHandler::ServerHandler(int port, const std::string &ip_address = "localhost") 
  : port {port}, ip_address {ip_address} {
  MainSocketAddress.sin_family = AF_INET;
  MainSocketAddress.sin_port = port;
  MainSocketAddress.sin_addr.s_addr = inet_addr(ip_address.c_str());
  main_socket_address_len = sizeof(MainSocketAddress);
}

void ServerHandler::setup_connection() {
  main_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (main_socket < 0) {
    oss << "Failed to create socket. Error number: " << errno << "\n";
    log(Log::EXIT);
  } else {
    log("Socket created successfully!!\n");
  }

  if (inet_pton(AF_INET, ip_address.c_str(), &MainSocketAddress.sin_addr.s_addr) <= 0) {
    oss << "Failed to assign ip. Error number: " << errno << "\n";
    log(Log::EXIT);
  } else {
    log("Ip assigned successfully!!\n");
  }

  if (bind(main_socket, (struct sockaddr *) &MainSocketAddress,
           sizeof(main_socket_address_len)) < 0) {
    oss << "Failed to bind the port " << port << ". Error number: " << errno << "\n";
    log(Log::EXIT);
  } else {
    log("Binding of the port was successfull\n");
  }
  
  if (listen(main_socket, 10) < 0) {
    oss << "Failed to listen to socket. Erron number: " << errno << "\n";
    log(Log::EXIT);
  } else {
    log("Listening to the socket\n");
  }
}

void ServerHandler::log(Log log_operation = Log::SUCCESS) {
  std::cout << oss.str() << std::endl;
  oss.clear();
  if (log_operation == Log::EXIT) {
    exit(EXIT_FAILURE);
  }
}

void ServerHandler::log(const std::string &log) const {
  std::cout << log << std::endl;
}

ServerHandler::~ServerHandler() {
  close(main_socket);
  close(connection);
}
