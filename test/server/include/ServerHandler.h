#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include <sstream>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

class ServerHandler {
public:
  ServerHandler(int port, const std::string &ip_address);

  void start_data_listener();
  void send_data(const std::string &response);

  void print_data(int bytes_read);
  ~ServerHandler();

private:
  int main_socket;
  int port;
  int connection;
  unsigned int main_socket_address_len;

  static const int BUFFER_SIZE = 30720;
  char buffer[BUFFER_SIZE];
  std::stringstream oss;
  std::string ip_address;

  struct sockaddr_in MainSocketAddress;
  void setup_connection();

  enum Log { EXIT, SUCCESS };
  void log(Log log_operation);
  void log(const std::string &log) const;
};

#endif // SERVER_HANDLER_H
