#ifndef AWS_S3_SETUP_HANDLER_HPP
#define AWS_S3_SETUP_HANDLER_HPP

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include "../../data/include/Chunk.hpp"
#include "../../metadata/include/MetaData.hpp"

using namespace Nexus;

class AwsS3SetupHandler {
protected:
  typedef std::unordered_map<std::string, std::vector<std::string>> BucketsParam;

  std::vector<std::unique_ptr<Chunk>> chunks;
  std::vector<std::pair<std::string, std::string>> buckets;
  std::unordered_map<std::string, std::pair<int, int>> bucketNumbers;

  MetaData metadata;

  void setChunks(const std::vector<std::unique_ptr<Chunk>> &original);

  void commonSetup();
  void storageSetup(const std::vector<std::unique_ptr<Chunk>> &chunks);
private: 
  // Common setup Helpers
  std::string defaultRegion;
  size_t numberOfBuckets;

  BucketsParam getExistingBuckets();
  void setBuckets(BucketsParam &buckets);

  void addNewBucketsToMetaData(BucketsParam &newBuckets);
  std::string getBucketLocation(const std::string &bucketName,
                                const Aws::S3::S3Client &s3Client);

  void setDefaultRegion();
  void setupBuckets();
  void setTotalNumberOfBuckets();

  // storage Setup Helpers
  const size_t hashFunction(const std::string &data, const size_t hashArgs = 0) const;

  // (primary bucket, replication bucket)
  std::unordered_map<std::string, size_t> getPrimaryBucketNums();
  std::unordered_map<std::string, size_t> getReplicationBucketNums(std::unordered_map<std::string, size_t> &primaryBucketNums);

  void setBucketNumbers();
  
  // Update the Chunk Metadata
  const size_t getBucketIdFromDatabase(const size_t &bucketNum);
  void updateChunkBucketMetaData();
};
#endif // AWS_S3_SETUP_HANDLER_HPP
