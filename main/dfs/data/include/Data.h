#ifndef DATA_H
#define DATA_H
#include <fstream>
#include <initializer_list>

class Data {
  Data(std::fstream &);
  Data(std::initializer_list<std::fstream> &);
};
#endif // DATA_H
