#include "cppcms_test/include/BasicTest.h"

int main(int argc, char **argv) {
  std::fstream file;
  std::ostringstream oss;

  try {
    std::string path_to_config_file = std::string(PATH_TO_SOURCES_FOLDER) + "config.json";
    file.open(path_to_config_file);

    if (!file.is_open()) {
      std::cout << "Error in opening " << path_to_config_file << std::endl;
      exit(EXIT_FAILURE);
    }

    oss << file.rdbuf();

    cppcms::service srv(argc, argv);
    srv.applications_pool().mount(
      cppcms::applications_factory<BasicTest>()
    );
    srv.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
