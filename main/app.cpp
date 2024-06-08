#include "dfs/include/Nexus.hpp"
#include "constants.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "server/include/Server.hpp"

int main() {
  Nexus::Server server;
  server.start();
  return 0;
}
