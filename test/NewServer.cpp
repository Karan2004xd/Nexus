#include <sstream>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sockaddr_in MainSocketAddress;
unsigned int main_socket_address_len;

const int PORT = 9999;
const std::string IP_ADDRESS = "127.0.0.1";
const int BUFFER_SIZE = 30720;

int main_socket;
int connection;
int buffer[BUFFER_SIZE];

void setup_connection(int port = PORT, const std::string &ip_address = IP_ADDRESS) {
  MainSocketAddress.sin_family = AF_INET;
  MainSocketAddress.sin_port = htons(port);
  MainSocketAddress.sin_addr.s_addr = inet_addr(ip_address.c_str());
  main_socket_address_len = sizeof(MainSocketAddress);

  main_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (main_socket < 0) {
    std::cout << "Failed to create socket. Error number: " << errno << std::endl;
  } else {
    std::cout << "Socket created successfully!!\n";
  }

  if (inet_pton(AF_INET, ip_address.c_str(), &MainSocketAddress.sin_addr.s_addr) <= 0) {
    std::cout << "Failed to assign ip. Error number: " << errno << std::endl;
  } else {
    std::cout << "Ip assigned successfully!!\n";
  }

  if (bind(main_socket, (struct sockaddr *) &MainSocketAddress,
           sizeof(MainSocketAddress)) < 0) {
    std::cout << "Failed to bind the port " << port << ". Error number: " << errno << std::endl;
  } else {
    std::cout << "Binding of the " << port << " was successfull\n";
  }

  if (listen(main_socket, 10) < 0) {
    std::cout << "Failed to listen to socket. Error number: " << errno << std::endl;
  } else {
    std::cout << "Listening to the socket\n";
  }
}

std::string generate_response() {
  std::ostringstream response;
  response << buffer;
  return response.str();
}

void send_response(const std::string &response) {

}

void listen_for_request() {
  while (true) {
    connection = accept(main_socket, (struct sockaddr *) &MainSocketAddress,
                        &main_socket_address_len);
    
    if (connection < 0) {
      std::cout << "Failed to connect. Error number: " << errno << "\n";
    } else {
      std::cout << "\nReading data now...";
    }
    
    while (true) {
      int bytes_read = read(connection, buffer, sizeof(buffer) - 1);
      if (bytes_read > 0) {
        std::string response = generate_response();
        send_response(response);
      }
    }
  }
}

int main() {
  return 0;
}
