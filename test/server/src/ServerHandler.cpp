#include "../include/ServerHandler.h"
#include "../include/SourcesHandler.h"

ServerHandler::ServerHandler(int port, const std::string &ip_address = "localhost") 
  : port {port}, ip_address {ip_address} {

  MainSocketAddress.sin_family = AF_INET;
  MainSocketAddress.sin_port = htons(port);
  MainSocketAddress.sin_addr.s_addr = inet_addr(ip_address.c_str());
  main_socket_address_len = sizeof(MainSocketAddress);

  setup_connection();
}

void ServerHandler::setup_connection() {
  main_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (main_socket < 0) {
    oss << "Failed to create socket. Error number: " << errno;
    log(Log::EXIT);
  } else {
    log("Socket created successfully!!\n");
  }

  if (inet_pton(AF_INET, ip_address.c_str(), &MainSocketAddress.sin_addr.s_addr) <= 0) {
    oss << "Failed to assign ip. Error number: " << errno;
    log(Log::EXIT);
  } else {
    log("Ip assigned successfully!!\n");
  }

  if (bind(main_socket, (struct sockaddr *) &MainSocketAddress,
           sizeof(MainSocketAddress)) < 0) {
    oss << "Failed to bind the port " << port << ". Error number: " << errno;
    log(Log::EXIT);
  } else {
    oss << "Binding of the " << port << " was successfull\n";
    log(Log::SUCCESS);
  }

  if (listen(main_socket, 10) < 0) {
    oss << "Failed to listen to socket. Error number: " << errno;
    log(Log::EXIT);
  } else {
    log("Listening to the socket\n");
  }
}

void ServerHandler::print_data(int bytes_read) {
  for (int i = 0; i < bytes_read; i++) {
    std::cout << buffer[i];
  }
  std::cout << "\n";
}

void ServerHandler::start_data_listener() {
  while (true) {
    connection = accept(main_socket, (struct sockaddr *) &MainSocketAddress,
                        &main_socket_address_len);
    
    if (connection < 0) {
      oss << "Failed to connect. Error number: " << errno << "\n";
      log(Log::EXIT);
    } else {
      log("\nReading data now...");
    }
    
    while (true) {
      int bytes_read = read(connection, buffer, sizeof(buffer) - 1);
      if (bytes_read > 0) {
        print_data(bytes_read);
        SourcesHandler sourcesHandler {buffer};
        send_data(sourcesHandler.get_response());
        std::cout << sourcesHandler.get_stored_data("username") << std::endl;
      }
    }
  }
}

void ServerHandler::send_data(const std::string &response) {
  int sending_response = send(connection, response.c_str(), response.length(), 0);
  if (sending_response < 0) {
    oss << "Failed to send the following response\n" << response;
    log(Log::EXIT);
  }
  /* std::cout << "Data sent" << std::endl; */
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
  log("Closed main socket");

  close(connection);
  log("Closed main connection");
}
