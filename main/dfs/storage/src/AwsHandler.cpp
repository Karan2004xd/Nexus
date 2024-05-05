#include "../include/AwsHandler.hpp"
#include "../../../constants.h"
#include <aws/s3/S3ClientConfiguration.h>

#include <aws/s3/S3ServiceClientModel.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <sstream>

using namespace Storage;
using namespace Aws;
using namespace Aws::Auth;

void AwsHandler::setupAwsConnection() {
  Aws::InitAPI(options);
  
  auto s3Client = getS3Client();
  auto outCome = s3Client.ListBuckets();

  if (outCome.IsSuccess()) {
    std::cout << "Connection established" << std::endl;
  } else {
    std::cerr << outCome.GetError() << std::endl;
    throw std::runtime_error("Error in AwsHandler : Connection cannot be established");
  }
}

std::string AwsHandler::getBucketName(int bucketNumber) {
  std::string result;
  switch (regions[bucketNumber]) {
    case StorageRegions::MAIN_US_EAST_1:
      result = std::string(BUCKET_MAIN_US_EAST_1_BUCKET);
      break;
    case StorageRegions::MAIN_US_EAST_2:
      result = std::string(BUCKET_MAIN_US_EAST_2_BUCKET);
      break;
    case StorageRegions::MAIN_AP_SOUTH_1:
      result = std::string(BUCKET_MAIN_AP_SOUTH_1);
      break;
    case StorageRegions::MAIN_AP_SOUTHEAST_1:
      result = std::string(BUCKET_MAIN_AP_SOUTHEAST_1);
      break;
    case StorageRegions::MAIN_EU_NORTH_1:
      result = std::string(BUCKET_MAIN_EU_NORTH_1);
      break;
    case StorageRegions::BACKUP_EU_WEST_2:
      result = std::string(BUCKET_BACKUP_EU_WEST_2);
      break;
  }
  return result;
}

Aws::S3::S3Client AwsHandler::getS3Client(int bucketNumber) {
  Aws::S3::S3ClientConfiguration config;
  switch (regions[bucketNumber]) {
    case StorageRegions::MAIN_US_EAST_1:
      config.region = Aws::Region::US_EAST_1;
      break;
    case StorageRegions::MAIN_US_EAST_2:
      config.region = Aws::Region::US_EAST_2;
      break;
    case StorageRegions::MAIN_AP_SOUTH_1:
      config.region = Aws::Region::AP_SOUTH_1;
      break;
    case StorageRegions::MAIN_AP_SOUTHEAST_1:
      config.region = Aws::Region::AP_SOUTHEAST_1;
      break;
    case StorageRegions::MAIN_EU_NORTH_1:
      config.region = Aws::Region::EU_NORTH_1;
      break;
    case StorageRegions::BACKUP_EU_WEST_2:
      config.region = Aws::Region::EU_WEST_2;
      break;
  }
  /* std::cout << config.region << std::endl; */
  return {config};
}

void AwsHandler::backupData(const std::string &objectKey,
                            const std::string &data) {
  const int bucketNumber = 5;
  std::string bucketName = getBucketName(bucketNumber);

  auto s3Client = getS3Client(bucketNumber);

  Aws::S3::Model::PutObjectRequest request;
  request.SetBucket(bucketName);
  request.SetKey(objectKey);

  auto dataStream = Aws::MakeShared<Aws::StringStream>("Data");
  *dataStream << data;
  request.SetBody(dataStream);

  auto putObjectOutcome = s3Client.PutObject(request);
  if (putObjectOutcome.IsSuccess()) {
    std::cout << "Object uploaded to the backup bucket" << std::endl;
  } else {
    std::cerr << "Failed to upload object to S3: " <<  putObjectOutcome.GetError() << std::endl;
    throw std::runtime_error("AwsHandler Error (backupData)");
  }
}

void AwsHandler::storeData(int bucketNumber,
                           const std::string &objectKey,
                           const std::string &data) {

  /* std::cout << bucketNumber << std::endl; */
  std::string bucketName = getBucketName(bucketNumber);
  /* std::cout << bucketName << std::endl; */

  auto s3Client = getS3Client(bucketNumber);

  Aws::S3::Model::PutObjectRequest request;
  request.SetBucket(bucketName);
  request.SetKey(objectKey);

  auto dataStream = Aws::MakeShared<Aws::StringStream>("Data");
  *dataStream << data;
  request.SetBody(dataStream);

  auto putObjectOutcome = s3Client.PutObject(request);
  if (putObjectOutcome.IsSuccess()) {
    std::cout << "Object uploaded successfully" << std::endl;
    /* backupData(objectKey, data); */
  } else {
    std::cerr << "Failed to upload object to S3: " <<  putObjectOutcome.GetError() << std::endl;
    throw std::runtime_error("AwsHandler Error (storeData)");
  }
}

void AwsHandler::deleteData(int bucketNumber,
                            const std::string &objectKey) {
  std::string bucketName = getBucketName(bucketNumber);
  
  auto s3Client = getS3Client(bucketNumber);

  Aws::S3::Model::DeleteObjectRequest request;
  request.WithKey(objectKey).WithBucket(bucketName);
  Aws::S3::Model::DeleteObjectOutcome outcome = s3Client.DeleteObject(request);
  
  if (outcome.IsSuccess()) {
    std::cout << "Successfully deleted object" << std::endl;
  } else {
    std::cerr << "Failed to delete the object: " << outcome.GetError() << std::endl;
    throw std::runtime_error("AwsHandler Error (deleteData)");
  }
}

std::string AwsHandler::getData(int bucketNumber, const std::string &objectKey) {
  std::ostringstream oss;
  std::string bucketName = getBucketName(bucketNumber);

  auto s3Client = getS3Client(bucketNumber);

  Aws::S3::Model::GetObjectRequest request;
  request.SetBucket(bucketName);
  request.SetKey(objectKey);

  Aws::S3::Model::GetObjectOutcome outcome = s3Client.GetObject(request);

  if (outcome.IsSuccess()) {
    oss << outcome.GetResult().GetBody().rdbuf();
  } else {
    std::cerr << "Failed to get the object data: " << outcome.GetError() << std::endl;
    throw std::runtime_error("AwsHandler Error (getData)");
  }
  return oss.str();
}

AwsHandler::AwsHandler() {
  setupAwsConnection();
  regions = {
    {0, StorageRegions::MAIN_US_EAST_1},
    {1, StorageRegions::MAIN_US_EAST_2},
    {2, StorageRegions::MAIN_AP_SOUTH_1},
    {3, StorageRegions::MAIN_AP_SOUTHEAST_1},
    {4, StorageRegions::MAIN_EU_NORTH_1},
    {5, StorageRegions::BACKUP_EU_WEST_2},
  };
}

AwsHandler::~AwsHandler() {
  Aws::ShutdownAPI(options);
}
