#include <iostream>
#include "TestClass.h"
/* #include <emscripten.h> */

// required for binding classes
/* #include <emscripten/bind.h> */ 
/* using namespace emscripten; */

// A SERVER IS A MUST FOR WASM TO WORK
// a simple hhtp server can be started using 
//  python -m http.server

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

/* EXTERN EMSCRIPTEN_KEEPALIVE int findSquare2(int a) { */
/*   return a * a; */
/* } */


/* class TryingWasm { */
/* private: */
/*   int a; */
/* public: */
/*   TryingWasm(int a) : a{a} {} */
/*   int findSquare() { */
/*     return a * a; */
/*   } */
/* }; */

/* TryingWasm obj; */
/* int result = obj.findSquare(10); */
/* EXTERN EMSCRIPTEN_KEEPALIVE int findSquare(int a) { */
/*   return result; */
/* } */

/* EMSCRIPTEN_BINDINGS (TryingWasm) { */
/*   class_<TryingWasm>("TryingWasm") */
/*     .constructor<int>() */
/*     .function("findSquare", &TryingWasm::findSquare); */
/* } */
// Very Scalable as you can see
/* class BaseClass { */
/* public: */
/*   int findCube(int num) { */
/*     return num * num * num; */
/*   } */
/* }; */

/* class AnotherClass { */
/* public: */
/*   AnotherClass() { */
/*     print_num(); */
/*   } */
/* private: */
/*   void print_num() { */
/*     std::cout << "Printing nums" << std::endl; */
/*     for (int i = 0; i < 20; i++) { */
/*       std::cout << i << " "; */
/*     } */
/*     std::cout << "\n"; */
/*   } */
/* }; */

/* class DerivedClass{ */
/* public: */
/*   DerivedClass() { */
/*     AnotherClass cls; */
/*   } */

/*   int findCube(int num) { */
/*     return baseClass.findCube(num); */
/*   } */
/* private: */
/*   BaseClass baseClass; */
/* }; */

/* EMSCRIPTEN_BINDINGS (DerivedClass) { */
/*   class_<DerivedClass>("DerivedClass") */
/*     .constructor() */
/*     .function("findCube", &DerivedClass::findCube); */
/* } */

// calling classes from header files
/* EMSCRIPTEN_BINDINGS (TestClass) { */
/*   class_<TestClass>("TestClass") */
/*     .constructor() */
/*     .function("findSquare", &TestClass::findSquare) */
/*     .function("findFactorial", &TestClass::findFactorial); */
    
/* } */

/* int main() { */
/*   TestClass t; */
/*   std::cout << t.findFactorial(5) << std::endl; */
/* } */
