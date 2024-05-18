#ifndef AWS_S3_HPP
#define AWS_S3_HPP
#include "../../dfs/include/Nexus.hpp"
#include "AwsCrudOperHandler.hpp"
#include "StorageBase.hpp"

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>

using namespace Nexus;

class Storage::AwsS3 : protected StorageBase, AwsCrudOperHandler {
public:
  AwsS3(const std::vector<std::unique_ptr<Chunk>> &chunks);

  void storeData(const std::string &data) override;
  std::string getData(const std::string &identifier) override;
  void deleteData(const std::string &identifier) override;

  ~AwsS3();

private:
  typedef std::unordered_map<std::string, std::vector<std::string>> BucketsParam;

  MetaData metadata;
  Aws::SDKOptions options;
  std::string defaultRegion;
  size_t numberOfBuckets;

  void setupStorage(const std::vector<std::unique_ptr<Chunk>> &chunks);

  // set Buckets
  // (Bucket Name, Bucket Region)
  std::vector<std::pair<std::string, std::string>> buckets;

  BucketsParam getExistingBuckets();
  void setBuckets(BucketsParam &buckets);

  void addNewBucketsToMetaData(BucketsParam &newBuckets);
  std::string getBucketLocation(const std::string &bucketName,
                                const Aws::S3::S3Client &s3Client);

  void setDefaultRegion();
  void setTotalNumberOfBuckets();
  void setupBuckets();

  // Assign Chunk to Bucket
  const size_t hashFunction(const std::string &data, const size_t hashArgs = 0) const;

  // (primary bucket, replication bucket)
  std::unordered_map<std::string, std::pair<int, int>> bucketNumbers;
  std::unordered_map<std::string, size_t> getPrimaryBucketNums();
  std::unordered_map<std::string, size_t> getReplicationBucketNums(std::unordered_map<std::string, size_t> &primaryBucketNums);

  void setBucketNumbers();

  // Update the Chunk Metadata
  const size_t getBucketIdFromDatabase(const size_t &bucketNum);
  void updateChunkBucketMetaData();

  Aws::S3::S3ClientConfiguration getConfig();
};
#endif // AWS_S3_HPP
