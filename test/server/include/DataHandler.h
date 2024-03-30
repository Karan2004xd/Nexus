#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <unordered_map>
#include <string>

class DataHandler {
public:
  void add_data(std::string &name, std::string &data);
  std::string get_data(std::string &name);
private:
  std::unordered_map<std::string, std::string> data;
};
#endif // DATA_HANDLER_H
