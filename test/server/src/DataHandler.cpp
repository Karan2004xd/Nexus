#include "../include/DataHandler.h"

void DataHandler::add_data(std::string &name, std::string &data) {
  this->data[name] = data;
}

std::string DataHandler::get_data(std::string &name) {
  std::string result {"NULL"};
  if (this->data.find(name) != this->data.end()) {
    result = this->data[name];
  }
  return result;
}
