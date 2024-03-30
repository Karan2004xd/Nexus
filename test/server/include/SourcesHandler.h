#ifndef SOURCES_HANDLER_H
#define SOURCES_HANDLER_H

#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdio>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "../../constants.h"
#include "DataHandler.h"

class SourcesHandler {
public:
  SourcesHandler(const char buffer[]);

  std::string get_response() const { return response.str(); }
  std::string get_stored_data(std::string name);

  ~SourcesHandler();

private:
  DataHandler dataHandler;
  enum DATA_PROTOCOL { POST, GET };

  std::fstream file;
  const std::string DEFAULT_FILE {"index.html"};
  const std::string default_response_start {"HTTP/1.1 "};

  std::stringstream response;
  std::string filename;
  std::unordered_map<std::string, std::string> file_types;

  std::unordered_map<int, std::string> status_codes;
  std::string filename_type;

  void populate_file_types();
  void set_file_data_required(const char buffer[]);

  bool set_file(std::string &filename);
  void populate_status_codes();

  void create_status_response(int status_code);
  std::string get_file_content() const;

  void create_response();
  void handle_get_data(const std::string &data);
  void handle_post_data(const std::string &data);

};
#endif // SOURCES_HANDLER_H
