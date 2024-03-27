#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H
#include <string>

class ConnectionHandler {
public:
  ConnectionHandler(int port, const std::string &ip_address = "localhost");
private:
  static const int BUFFER_SIZE = 30720;
  int main_socket;
  int port;
};

#endif // CONNECTION_HANDLER_H
