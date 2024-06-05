#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../dfs/include/Nexus.hpp"
#include "ServerHandler.hpp"

class Nexus::Server {
public:
  void start();
private:
  ServerHandler serverHandler;
};
#endif // SERVER_HPP
