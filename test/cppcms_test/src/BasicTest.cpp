#include "../include/BasicTest.h"

BasicTest::BasicTest(cppcms::service &srv)
  : cppcms::application(srv) {}

void BasicTest::main(std::string url) {
  std::string file_path = std::string(PATH_TO_SOURCES_FOLDER) + "index.html";
  file.open(file_path);

  if (!file.is_open()) {
    std::cout << "Error in opening " << file_path << std::endl;
    exit(EXIT_FAILURE);
  }

  oss << file.rdbuf();
  /* std::cout << oss.str() << std::endl; */

  /* response().out() << */ 
  /*   "<html>\n" */
  /*   "<body>\n" */
  /*   "  <h1>Hello World</h1>\n" */
  /*   "<body>\n" */
  /*   "<html>\n"; */
  response().out() << oss.str();
}
