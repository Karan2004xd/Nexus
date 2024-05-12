#ifndef SERVER_HPP
#define SERVER_HPP
#include "../../dfs/include/Nexus.hpp"
#include "ServerHandler.hpp"

using namespace Nexus;

class Server : protected ServerHandler {
public:
  static void init();
};
#endif // SERVER_HPP
