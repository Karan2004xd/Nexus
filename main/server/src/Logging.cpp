#include "../include/Logging.hpp"

void Logging::init() {
  logging::core::get()->set_logging_enabled(true);
}
