#ifndef AWS_HANDLER_HPP
#define AWS_HANDLER_HPP

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include "Storage.hpp"

// DON'T USE OR DECLARE ANY OTHER API'S BEFORE USING InitApi for Aws

class Storage::AwsHandler {
public:
  AwsHandler();

  void storeData(int, const std::string &, const std::string &);
  void deleteData(int, const std::string &);
  std::string getData(int, const std::string &);

  ~AwsHandler();
private:
  Aws::SDKOptions options;

  enum StorageRegions {
    MAIN_US_EAST_1, // N. Virginia
    MAIN_US_EAST_2, // Ohio
    MAIN_AP_SOUTH_1, // Mumbai
    MAIN_AP_SOUTHEAST_1, // Singapore
    MAIN_EU_NORTH_1, // Stockholm
    BACKUP_EU_WEST_2 // London
  };

  std::string getBucketName(int);
  Aws::S3::S3Client getS3Client(int = -1);

  void backupData(const std::string &, const std::string &);

  std::unordered_map<int, StorageRegions> regions;
  void setupAwsConnection();
};
#endif // AWS_HANDLER_HPP
