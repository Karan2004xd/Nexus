#include "../include/Metadata.hpp"

using namespace Database;

Metadata::Metadata() {
  DatabaseHandler::checkConnection();
}
