#include "../include/AwsS3.hpp"

#include <algorithm>
#include <aws/s3/S3ClientConfiguration.h>
#include <aws/s3/S3ServiceClientModel.h>

#include <aws/s3/model/GetBucketLocationRequest.h>
#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/BucketLocationConstraint.h>
#include <sstream>

#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"

using namespace Storage;
using namespace Aws;
using namespace Aws::Auth;

void AwsS3::setupStorage(const std::vector<std::unique_ptr<Chunk>> &chunks) {
  setChunks(chunks);
  setDefaultRegion();
  setupBuckets();
  setTotalNumberOfBuckets();
  setBucketNumbers();
  updateChunkBucketMetaData();
}

AwsS3::AwsS3(const std::vector<std::unique_ptr<Chunk>> &chunks) {
  Aws::InitAPI(this->options);
  setupStorage(chunks);
}

void AwsS3::storeData(const std::string &data) {
}

std::string AwsS3::getData(const std::string &identifier) {

}

void AwsS3::deleteData(const std::string &identifier) {

}

AwsS3::BucketsParam AwsS3::getExistingBuckets() {
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

void AwsS3::setBuckets(BucketsParam &buckets) {
  size_t lenghtOfColumn = buckets["name"].size();
  for (int i = 0; i < lenghtOfColumn; i++) {
    auto newPair = std::make_pair(buckets["name"][i], buckets["region"][i]);
    this->buckets.push_back(newPair);
  }
}

void AwsS3::addNewBucketsToMetaData(BucketsParam &newBuckets) {
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

std::string AwsS3::getBucketLocation(const std::string &bucketName,
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
    oss << "(AwsS3) : Error while fetching the bucket location"
        << "\nError: " << getBucketLocationOutcome.GetError().GetMessage();
    throw std::runtime_error(oss.str());
  }
  return location;
}

void AwsS3::setDefaultRegion() {
  Aws::Client::ClientConfiguration config;
  this->defaultRegion = config.region;
}

void AwsS3::setTotalNumberOfBuckets() {
  this->numberOfBuckets = this->buckets.size();
}

void AwsS3::setupBuckets() {
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
    oss << "(AwsS3) : Error while listing buckets"
        << "\nError : " << listBucketsOutcome.GetError().GetMessage();
    throw std::runtime_error(oss.str());
  }
}

const size_t AwsS3::hashFunction(const std::string &data, const size_t hashArgs) const {
  std::hash<std::string> strHash;
  uint64_t hash = strHash(data);
  return (hash + hashArgs) % this->numberOfBuckets;
}

std::unordered_map<std::string, size_t> AwsS3::getPrimaryBucketNums() {
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

std::unordered_map<std::string, size_t> AwsS3::getReplicationBucketNums(std::unordered_map<std::string, size_t> &primaryBucketNums) {
  std::unordered_map<std::string, size_t> replicationBucketNums;
  for (const auto &chunk : chunks) {
    size_t replicationNum = hashFunction(chunk->getEncryptedData());
    size_t i = 1;

    while (checkForValueInMap(primaryBucketNums, replicationNum)) {
      replicationNum = hashFunction(chunk->getEncryptedData(), i * i);
      /* std::cout << replicationNum << std::endl; */
      i++;
    }
    replicationBucketNums[chunk->getObjectKey()] = replicationNum;
  }
  return replicationBucketNums;
}

void AwsS3::setBucketNumbers() {
  auto primaryBucketNums = getPrimaryBucketNums();
  auto replicationBucketNums = getReplicationBucketNums(primaryBucketNums);
  
  for (const auto &chunk : chunks) {
    std::string objectKey = chunk->getObjectKey();
    auto bucketPair = std::make_pair(primaryBucketNums[objectKey],
                                     replicationBucketNums[objectKey]);
    std::cout << bucketPair.first << " " << bucketPair.second << std::endl;
    this->bucketNumbers[objectKey] = bucketPair;
  }
}

const size_t AwsS3::getBucketIdFromDatabase(const size_t &bucketNum) {
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

void AwsS3::updateChunkBucketMetaData() {
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

AwsS3::~AwsS3() {
  Aws::ShutdownAPI(options);
}
