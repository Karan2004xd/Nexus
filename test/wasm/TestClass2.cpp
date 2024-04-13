#include "TestClass2.h"

int TestClass2::factorial(int num, int result) {
  if (num <= 0) {
    return result;
  }
  result = result * num;
  num--;
  return factorial(num, result);
}
