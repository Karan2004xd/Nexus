#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <iostream>
class TestClass {
public:
  TestClass() {
    std::cout << "Hello, from test class" << std::endl;
  }

  int findSquare(int a) {
    return a * a;
  }
};
#endif // TESTCLASS_H

