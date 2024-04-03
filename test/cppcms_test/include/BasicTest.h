#ifndef BASIC_TEST_H
#define BASIC_TEST_H

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <sstream>

#include "../../constants.h"

class BasicTest : public cppcms::application {
public:
  BasicTest(cppcms::service &srv);
  void main(std::string url);
private:
  std::fstream file;
  std::ostringstream oss;
};
#endif // BASIC_TEST_H
