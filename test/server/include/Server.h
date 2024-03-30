#ifndef SERVER_H
#define SERVER_H
#include <memory>

#include "ServerHandler.h"

class Server {
public:
  Server(int port, const std::string &ip_address);
  Server(int port);
  Server();

  void start_server();
private:
  std::unique_ptr<ServerHandler> serverHandler;
  const std::string IP_ADDRESS = {"127.0.0.1"};
  const int PORT = 9999;
};
#endif // SERVER_H
