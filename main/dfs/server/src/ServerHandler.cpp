#include "../include/ServerHandler.hpp"

using severity_level = logging::trivial::severity_level;

void ServerHandler::log(const std::string &msg, enum logging::trivial::severity_level info) {
  BOOST_LOG_TRIVIAL(info) << msg;
}

void ServerHandler::setupConnection() {
  try {
  } catch (const std::exception &e) {
    log(e.what(), severity_level::fatal);
  }
}
