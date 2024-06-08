#ifndef OPERATION_EXCEPTION_HPP
#define OPERATION_EXCEPTION_HPP

#include <string>
#include <exception>

class OperationException : public std::exception {
private:
  std::string msg;
public:
  explicit OperationException(const std::string &msg) : msg(msg) {}

  const char *what() const noexcept override {
    return msg.c_str();
  }
};
#endif // OPERATION_EXCEPTION_HPP
