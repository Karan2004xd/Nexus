#include "../include/SourcesHandler.h"

SourcesHandler::SourcesHandler(const char buffer[]) {
  populate_file_types(); 
  set_file_data_required(buffer); 
  create_response();
}

void SourcesHandler::populate_file_types() {
  this->file_types = {
    {"html", "text/html"},
    {"js", "application/javascript"},
    {"css", "text/css"},
    {"post", "application/x-www-form-urlencoded"}
  };
}

void SourcesHandler::populate_status_codes() {
  this->status_codes = {
    {404, "Not Found\r\nContent-Length: 0\r\n\r\n"},
    {200, "OK\r\n"}
  };
}

void SourcesHandler::handle_get_data(const std::string &data) {
  size_t pos = data.find('=');
  if (pos != std::string::npos) {
    std::string username = data.substr(0, pos);
    std::string username_data = data.substr(pos + 1);

    while (true) {
      pos = username_data.find('+');
      if (pos != std::string::npos) {
        username_data[pos] = ' ';
      } else {
        break;
      }
    }
    dataHandler.add_data(username, username_data);
  }
}

void SourcesHandler::handle_post_data(const std::string &data) {
  std::vector<std::string> lines;
  boost::split(lines, data, boost::is_any_of("\n"));
  if (!lines.empty()) {
    std::string last_line = lines[lines.size() - 1];
    handle_get_data(last_line);
  }
}

void SourcesHandler::set_file_data_required(const char buffer[]) {
  std::istringstream iss {buffer};
  std::string method, path, temp_filename;

  if (iss >> method >> path) {
    temp_filename = path.substr(1);
  } else {
    temp_filename = "";
  }

  if (temp_filename.find(".js") != std::string::npos) {
    this->filename_type = "js";
  } else if (temp_filename.find(".css") != std::string::npos) {
    this->filename_type = "css";
  } else if (temp_filename.find('?') != std::string::npos) {
    temp_filename[0] = '\0';
    handle_get_data(temp_filename);
    return ;
  } else if (iss.str().find(file_types["post"]) != std::string::npos) {
    handle_post_data(iss.str());
    return ;
  } else {
    this->filename_type = "html";
  }
  this->filename = temp_filename;
}

void SourcesHandler::create_status_response(int status_code) {
  response << default_response_start << status_code << " " << status_codes[status_code] << "\n";
}

std::string SourcesHandler::get_file_content() const {
  std::stringstream oss;
  oss << file.rdbuf();
  return oss.str();
}

void SourcesHandler::create_response() {
  if (set_file(filename)) {
    std::string content = get_file_content();
    create_status_response(200);

    response << "Content-Type: " << file_types[filename_type] << "; charset=UTF-8\r\n"
             << "Content-Length: " << content.length() << "\r\n\r\n"
             << content;
             /* << "\r\nCache-Control: max-age=3600\r\n"; */
  }
}

bool SourcesHandler::set_file(std::string &filename) {
  if (filename.empty()) {
    filename = DEFAULT_FILE;
  }

  std::string file_path = std::string(PATH_TO_SOURCES_FOLDER) + filename;
  file.open(file_path);
  if (!file.is_open()) {
    std::cout << "Error in opening the file with path (" + file_path + ")" << std::endl;
    create_status_response(404);
    return false;
  }
  return true;
}

std::string SourcesHandler::get_stored_data(std::string name) {
  return dataHandler.get_data(name);
}

SourcesHandler::~SourcesHandler() {
  file.close();
  response.clear();
}
