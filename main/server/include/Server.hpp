#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../dfs/include/Nexus.hpp"
#include "ServerHandler.hpp"

class Nexus::Server : protected ServerHandler {
public:
  static void run();
};

#endif // SERVER_HPP
