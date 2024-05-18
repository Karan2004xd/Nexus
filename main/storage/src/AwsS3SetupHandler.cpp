#include "../include/AwsS3SetupHandler.hpp"

#include <aws/s3/model/GetBucketLocationRequest.h>
#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/BucketLocationConstraint.h>

#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"

void AwsS3SetupHandler::setDefaultRegion() {
  Aws::Client::ClientConfiguration config;
  this->defaultRegion = config.region;
}

void AwsS3SetupHandler::setTotalNumberOfBuckets() {
  this->numberOfBuckets = this->buckets.size();
}

void AwsS3SetupHandler::setBuckets(BucketsParam &buckets) {
  size_t lenghtOfColumn = buckets["name"].size();
  for (int i = 0; i < lenghtOfColumn; i++) {
    auto newPair = std::make_pair(buckets["name"][i], buckets["region"][i]);
    this->buckets.push_back(newPair);
  }
}

std::string AwsS3SetupHandler::getBucketLocation(const std::string &bucketName,
                                                 const Aws::S3::S3Client &s3Client) {
  std::string location;
  std::ostringstream oss;

  Aws::S3::Model::GetBucketLocationRequest locationRequest;
  locationRequest.SetBucket(bucketName);

  auto getBucketLocationOutcome = s3Client.GetBucketLocation(locationRequest);
  if (getBucketLocationOutcome.IsSuccess()) {
    auto locationConstrain = getBucketLocationOutcome.GetResult().GetLocationConstraint();

    if (locationConstrain == Aws::S3::Model::BucketLocationConstraint::NOT_SET) {
      location = defaultRegion;
    } else {
      location = Aws::S3::Model::BucketLocationConstraintMapper::GetNameForBucketLocationConstraint(locationConstrain);
    }
  } else {
    oss << "(AwsS3SetupHandler) : Error while fetching the bucket location"
      << "\nError: " << getBucketLocationOutcome.GetError().GetMessage();
    throw std::runtime_error(oss.str());
  }
  return location;
}

AwsS3SetupHandler::BucketsParam AwsS3SetupHandler::getExistingBuckets() {
  BucketsParam result;
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetBuckets")
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);

  for (const auto &key : queryOutput) {
    result[key.first] = key.second;
  }
  return result;
}

void AwsS3SetupHandler::addNewBucketsToMetaData(BucketsParam &newBuckets) {
  size_t lenghtOfColumn = newBuckets["name"].size();
  for (int i = 0; i < lenghtOfColumn; i++) {
    auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
      .singleData("file", "InsertBucket")
      .singleData("name", newBuckets["name"][i])
      .singleData("region", newBuckets["region"][i])
      .getJsonData();

    auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
    /* std::cout << queryData.getParsedData() << std::endl; */
    metadata.updateData(queryData);
  }
}

void AwsS3SetupHandler::setupBuckets() {
  std::ostringstream oss;

  auto existingBuckets = getExistingBuckets();
  BucketsParam newBuckets = {
    {"name", {}},
    {"region", {}}
  };

  Aws::S3::S3Client s3Client;

  Aws::S3::Model::ListBucketsOutcome listBucketsOutcome = s3Client.ListBuckets();

  if (listBucketsOutcome.IsSuccess()) {
    const auto &buckets = listBucketsOutcome.GetResult().GetBuckets();
    std::vector<Aws::S3::Model::Bucket> bucketList (buckets.begin(), buckets.end());

    for (const auto &bucket : bucketList) {
      std::string bucketName = bucket.GetName();
      std::vector<std::string> bucketColumn = existingBuckets["name"];
      auto it = std::find(bucketColumn.begin(), bucketColumn.end(),
                          bucketName);

      if (it == bucketColumn.end()) {
        std::string location = getBucketLocation(bucketName, s3Client);
        newBuckets["name"].push_back(bucketName);
        newBuckets["region"].push_back(location);
        std::cout << bucketName << " " << location << std::endl;
      }
    }

    size_t newBucketsColumnSize = newBuckets["name"].size();
    if (newBucketsColumnSize > 0) {
      addNewBucketsToMetaData(newBuckets);
      for (int i = 0; i < newBucketsColumnSize; i++) {
        existingBuckets["name"].push_back(newBuckets["name"][i]);
        existingBuckets["region"].push_back(newBuckets["region"][i]);
      }
    }

    setBuckets(existingBuckets);

  } else {
    oss << "(AwsS3SetupHandler) : Error while listing buckets"
      << "\nError : " << listBucketsOutcome.GetError().GetMessage();
    throw std::runtime_error(oss.str());
  }
}

void AwsS3SetupHandler::setChunks(const std::vector<std::unique_ptr<Chunk>> &original) {
  for (const auto &chunk : original) {
    this->chunks.push_back(std::make_unique<Chunk>(*chunk->clone()));
  }
}

void AwsS3SetupHandler::commonSetup() {
  setDefaultRegion();
  setupBuckets();
  setTotalNumberOfBuckets();
}

// Methods for storage Setup

const size_t AwsS3SetupHandler::hashFunction(const std::string &data, const size_t hashArgs) const {
  std::hash<std::string> strHash;
  uint64_t hash = strHash(data);
  return (hash + hashArgs) % this->numberOfBuckets;
}

std::unordered_map<std::string, size_t> AwsS3SetupHandler::getPrimaryBucketNums() {
  std::unordered_map<std::string, size_t> primaryBucketNums;
  for (const auto &chunk : chunks) {
    size_t hashValue = hashFunction(chunk->getEncryptedData());
    std::string objectKey = chunk->getObjectKey();

    primaryBucketNums[objectKey] = hashValue;
  }
  return primaryBucketNums;
}

template <typename T1, typename T2>
bool checkForValueInMap(std::unordered_map<T1, T2> &map,
                        const T2 &value) {
  for (const auto &key : map) {
    if (key.second == value) return true;
  }
  return false;
}

std::unordered_map<std::string, size_t> AwsS3SetupHandler::getReplicationBucketNums(std::unordered_map<std::string, size_t> &primaryBucketNums) {
  std::unordered_map<std::string, size_t> replicationBucketNums;
  for (const auto &chunk : chunks) {
    size_t replicationNum = hashFunction(chunk->getEncryptedData());
    size_t i = 1;

    while (checkForValueInMap(primaryBucketNums, replicationNum)) {
      replicationNum = hashFunction(chunk->getEncryptedData(), i * i);
      i++;
    }
    while (replicationNum >= this->buckets.size()) {
      replicationNum--;
    }
    replicationBucketNums[chunk->getObjectKey()] = replicationNum;
  }
  return replicationBucketNums;
}

void AwsS3SetupHandler::setBucketNumbers() {
  auto primaryBucketNums = getPrimaryBucketNums();
  auto replicationBucketNums = getReplicationBucketNums(primaryBucketNums);
  
  for (const auto &chunk : chunks) {
    std::string objectKey = chunk->getObjectKey();
    auto bucketPair = std::make_pair(primaryBucketNums[objectKey],
                                     replicationBucketNums[objectKey]);
    /* std::cout << bucketPair.first << " " << bucketPair.second << std::endl; */
    this->bucketNumbers[objectKey] = bucketPair;
  }
}

const size_t AwsS3SetupHandler::getBucketIdFromDatabase(const size_t &bucketNum) {
  std::string bucketName = this->buckets[bucketNum].first;
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetBucketId")
    .singleData("name", bucketName)
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);
  size_t bucketId = std::stoi(queryOutput["id"][0]);
  return bucketId;
}

void AwsS3SetupHandler::updateChunkBucketMetaData() {
  for (const auto &key : this->bucketNumbers) {
    size_t primaryBucketId = getBucketIdFromDatabase(key.second.first);
    size_t replicationBucketId = getBucketIdFromDatabase(key.second.second);

    auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
      .singleData("file", "UpdateChunkBucketInfo")
      .singleData("primary_bucket_id", std::to_string(primaryBucketId))
      .singleData("replicated_bucket_id", std::to_string(replicationBucketId))
      .singleData("object_key", key.first)
      .getJsonData();

    auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
    metadata.updateData(queryData);
  }
}

void AwsS3SetupHandler::storageSetup(const std::vector<std::unique_ptr<Chunk>> &chunks) {
  setChunks(chunks);
  setBucketNumbers();
  updateChunkBucketMetaData();
}
