#include "../include/AwsS3.hpp"

#include <algorithm>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/s3/S3ClientConfiguration.h>
#include <aws/s3/S3ServiceClientModel.h>

#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/BucketLocationConstraint.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetBucketLocationRequest.h>
#include <sstream>

#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"

using namespace Storage;
using namespace Aws;
using namespace Aws::Auth;

AwsS3::AwsS3(const std::vector<std::unique_ptr<Chunk>> &chunks) {
  Aws::InitAPI(this->options);
  this->chunks = this->copyChunks(chunks);
  setupBuckets();
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
      location = "us-east-1";
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
        /* std::cout << bucketName << " " << location << std::endl; */
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

AwsS3::~AwsS3() {
  Aws::ShutdownAPI(options);
}
