#ifndef AWS_S3_HPP
#define AWS_S3_HPP
#include "../../dfs/include/Nexus.hpp"
#include "StorageBase.hpp"

using namespace Nexus;

class Storage::AwsS3 : protected StorageBase {
private:
  void storeData(const std::string &data) override;
  std::string getData(const std::string &identifier) override;
  void deleteData(const std::string &identifier) override;
};
#endif // AWS_S3_HPP
