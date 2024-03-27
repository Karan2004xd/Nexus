#include <iostream>

struct Base {
  int num;
  Base(int num) : num {num} {}
};

struct Derived {
  Base base {5};
};

int main() {
  Derived d;
  std::cout << d.base.num << std::endl;
  return 0;
}
