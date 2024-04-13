#ifndef TESTCLASS_H
#define TESTCLASS_H
#include "TestClass2.h"

#include <iostream>

class TestClass {
private:
  TestClass2 testClass2;
public:
  TestClass() {
    std::cout << "Hello, from test class" << std::endl;
  }
  int findSquare(int a) {
    return a * a;
  }

  int findFactorial(int a) {
    return testClass2.factorial(a);
  }
};
#endif // TESTCLASS_H

