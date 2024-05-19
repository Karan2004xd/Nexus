#include "../include/AwsS3.hpp"

#include <aws/s3/S3ClientConfiguration.h>
#include <aws/s3/S3Errors.h>
#include <aws/s3/S3ServiceClientModel.h>

#include <aws/s3/model/GetBucketLocationRequest.h>
#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/BucketLocationConstraint.h>

#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"

using namespace Storage;
using namespace Aws;
using namespace Aws::Auth;

AwsS3::AwsS3() {
  Aws::InitAPI(options);
  commonSetup();
}

Aws::S3::S3ClientConfiguration AwsS3::getConfig(const std::string &region) {
  Aws::S3::S3ClientConfiguration config;
  config.region = region;
  return config;
}

void AwsS3::storeDataHelper(const std::string &objectKey,
                            const std::string &data) {
  size_t primaryBucketId = this->bucketNumbers[objectKey].first;
  size_t replicationBucketId = this->bucketNumbers[objectKey].second;

  std::string primaryBucketName = this->buckets[primaryBucketId].first;
  std::string primaryBucketRegion = this->buckets[primaryBucketId].second;

  std::string replicationBucketName = this->buckets[replicationBucketId].first;
  std::string replicationBucketRegion = this->buckets[replicationBucketId].second;

  auto primaryBucketConfig = getConfig(primaryBucketRegion);
  auto replicationBucketConfig = getConfig(replicationBucketRegion);

  putData(primaryBucketConfig, objectKey, primaryBucketName, data);
  putData(replicationBucketConfig, objectKey, replicationBucketName, data);
}

void AwsS3::storeData(const std::vector<std::unique_ptr<Chunk>> &chunks) {
  storageSetup(chunks);
  std::vector<std::future<void>> activeThreads;

  for (const auto &chunk : chunks) {
    std::string objectKey = chunk->getObjectKey();
    std::string data = chunk->getEncryptedData();

    activeThreads.push_back(std::async(
      std::launch::async, &AwsS3::storeDataHelper, this, objectKey, data
    ));
  }

  for (const auto &thread : activeThreads) {
    thread.wait();
  }
}

MetaData::QueryResultMap AwsS3::getDataHelper(const size_t &fileId) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetDataQuery")
    .singleData("file_id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);

  if (queryOutput["chunk_key"].size() <= 0) {
    throw std::runtime_error("(AwsS3) : No chunks found in getDataHelper, for the given file id");
  }
  return queryOutput;
}

std::unique_ptr<Chunk> AwsS3::getDataHelper2(MetaData::QueryResultMap &queryOutput,
                                             int i,
                                             const BucketType &bucketType) {
  std::string chunkKey = queryOutput.at("chunk_key")[i];
  std::string objectKey = queryOutput.at("object_key")[i];
  std::string data;

  if (bucketType == BucketType::PRIMARY) {
    size_t primaryBucketId = std::stoi(queryOutput.at("primary_bucket_id")[i]) - 1;

    std::string primaryBucketName = this->buckets.at(primaryBucketId).first;
    std::string primaryBucketRegion = this->buckets.at(primaryBucketId).second;

    auto primaryBucketConfig = getConfig(primaryBucketRegion);
    try {
      data = AwsCrudOperHandler::getData(primaryBucketConfig, primaryBucketName, objectKey);
    } catch (const std::exception &error) {
      std::string errorStr = error.what();
      if (errorStr == "SERVICE FAILURE") {
        getDataHelper2(queryOutput, i, BucketType::REPLICATION);
      }
    }
  } else if (bucketType == BucketType::REPLICATION) {
    size_t replicationBucketId = std::stoi(queryOutput.at("replicated_bucket_id")[i]) - 1;

    std::string replicationBucketName = this->buckets.at(replicationBucketId).first;
    std::string replicationBucketRegion = this->buckets.at(replicationBucketId).second;

    auto replicationBucketConfig = getConfig(replicationBucketRegion);
    data = AwsCrudOperHandler::getData(replicationBucketConfig, replicationBucketName, objectKey);
  }
  return std::make_unique<Chunk>(data, chunkKey);
}

std::vector<std::unique_ptr<Chunk>> AwsS3::getData(const size_t &fileId) {
  std::vector<std::unique_ptr<Chunk>> result;
  std::vector<std::future<std::unique_ptr<Chunk>>> activeThreads;

  auto queryOutput = getDataHelper(fileId);
  size_t lenghtOfColumn = queryOutput["chunk_key"].size();

  std::cout << "Fetching Data" << std::endl;

  for (int i = 0; i < lenghtOfColumn; i++) {
    activeThreads.push_back(
      std::async(&AwsS3::getDataHelper2, this,
                 std::ref(queryOutput), i, BucketType::PRIMARY)
    );
  }

  for (auto &thread : activeThreads) {
    thread.wait();
    result.push_back(thread.get());
  }
  return result;
}

std::vector<std::unique_ptr<Chunk>> AwsS3::getBackupData(const size_t &fileId) {
  std::vector<std::unique_ptr<Chunk>> result;
  std::vector<std::future<std::unique_ptr<Chunk>>> activeThreads;

  auto queryOutput = getDataHelper(fileId);
  size_t lenghtOfColumn = queryOutput["chunk_key"].size();

  std::cout << "Fetching Data" << std::endl;

  for (int i = 0; i < lenghtOfColumn; i++) {
    activeThreads.push_back(
      std::async(&AwsS3::getDataHelper2, this,
                 std::ref(queryOutput), i, BucketType::REPLICATION)
    );
  }

  for (auto &thread : activeThreads) {
    thread.wait();
    result.push_back(thread.get());
  }
  return result;
}

void AwsS3::restoreData(const std::vector<std::unique_ptr<Chunk>> &chunks, 
                        const size_t &fileId) {
  std::vector<std::future<std::unique_ptr<Chunk>>> activeThreads;
  deleteBackupData(fileId);
  storeData(chunks);
}

void AwsS3::deleteDataHelper(MetaData::QueryResultMap &queryOutput,
                             int i,
                             const BucketType &bucketType) {
  size_t primaryBucketId = std::stoi(queryOutput.at("primary_bucket_id")[i]) - 1;
  size_t replicationBucketId = std::stoi(queryOutput.at("replicated_bucket_id")[i]) - 1;

  std::string objectKey = queryOutput.at("object_key")[i];

  std::string primaryBucketName = this->buckets.at(primaryBucketId).first;
  std::string primaryBucketRegion = this->buckets.at(primaryBucketId).second;

  std::string replicationBucketName = this->buckets.at(replicationBucketId).first;
  std::string replicationBucketRegion = this->buckets.at(replicationBucketId).second;

  auto primaryBucketConfig = getConfig(primaryBucketRegion);
  auto replicationBucketConfig = getConfig(replicationBucketRegion);

  if (bucketType == BucketType::PRIMARY) {
    AwsCrudOperHandler::deleteData(primaryBucketConfig, primaryBucketName, objectKey);
  } else if (bucketType == BucketType::REPLICATION) {
    AwsCrudOperHandler::deleteData(replicationBucketId, replicationBucketName, objectKey);
  }
}

void AwsS3::updateMetaDataFordeletedData(const BucketType &bucketType,
                                         const size_t &fileId) {
  std::string queryFile;
  if (bucketType == BucketType::PRIMARY) {
    queryFile = "UpdatePrimaryBucketNumber";
  } else if (bucketType == BucketType::REPLICATION) {
    queryFile = "DeleteDataQuery";
  }
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", queryFile)
    .singleData("file_id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  metadata.updateData(queryData);
}

void AwsS3::deleteData(const size_t &fileId) {
  std::vector<std::future<void>> activeThreads;

  auto queryOutput = getDataHelper(fileId);
  size_t lenghtOfColumn = queryOutput["chunk_key"].size();

  for (int i = 0; i < lenghtOfColumn; i++) {
    activeThreads.push_back(
      std::async(&AwsS3::deleteDataHelper, this,
                 std::ref(queryOutput), i, BucketType::PRIMARY)
    );
  }
  for (auto &thread : activeThreads) {
    thread.wait();
  }
  updateMetaDataFordeletedData(BucketType::PRIMARY, fileId);
}

void AwsS3::deleteBackupData(const size_t &fileId) {
  std::vector<std::future<void>> activeThreads;

  auto queryOutput = getDataHelper(fileId);
  size_t lenghtOfColumn = queryOutput["chunk_key"].size();

  for (int i = 0; i < lenghtOfColumn; i++) {
    activeThreads.push_back(
      std::async(&AwsS3::deleteDataHelper, this,
                 std::ref(queryOutput), i, BucketType::REPLICATION)
    );
  }
  for (auto &thread : activeThreads) {
    thread.wait();
  }
  updateMetaDataFordeletedData(BucketType::REPLICATION, fileId);
}

AwsS3::~AwsS3() {
  Aws::ShutdownAPI(options);
}
