#ifndef DATA_HPP
#define DATA_HPP

#include <initializer_list>
#include <string>

class Data {
  Data(const std::string &);
  Data(std::initializer_list<std::string> &);
};
#endif // DATA_HPP
