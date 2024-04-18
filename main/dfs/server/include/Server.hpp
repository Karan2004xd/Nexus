#ifndef SERVER_HPP
#define SERVER_HPP

#include <memory>
#include "ServerHandler.hpp"

class Server {
public:
  Server();
  void start();
private:
  std::unique_ptr<ServerHandler> serverHandler;
};
#endif // SERVER_HPP
