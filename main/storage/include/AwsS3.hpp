#ifndef AWS_S3_HPP
#define AWS_S3_HPP
#include "../../dfs/include/Nexus.hpp"
#include "StorageBase.hpp"

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>

using namespace Nexus;

class Storage::AwsS3 : protected StorageBase {
public:
  AwsS3(const std::vector<std::unique_ptr<Chunk>> &chunks);

  void storeData(const std::string &data) override {}
  std::string getData(const std::string &identifier) override { return {}; }
  void deleteData(const std::string &identifier) override {}

  ~AwsS3();

private:
  typedef std::unordered_map<std::string, std::vector<std::string>> BucketsParam;

  MetaData metadata;
  Aws::SDKOptions options;

  // set Buckets
  std::vector<std::pair<std::string, std::string>> buckets;

  BucketsParam getExistingBuckets();
  void setBuckets(BucketsParam &buckets);
  void addNewBucketsToMetaData(BucketsParam &newBuckets);
  std::string getBucketLocation(const std::string &bucketName,
                                const Aws::S3::S3Client &s3Client);
  void setupBuckets();

  // Assign Chunk to Bucket
};
#endif // AWS_S3_HPP
