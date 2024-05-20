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

MetaData::QueryResultMap AwsS3::getDataHelper(const size_t &fileId,
                                              const BucketType &bucketType) {
  std::string queryFile;
  if (bucketType == BucketType::PRIMARY) {
    queryFile = "GetDataQuery";
  } else if (bucketType == BucketType::REPLICATION) {
    queryFile = "GetTrashDataQuery";
  }
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", queryFile)
    .singleData("file_id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);
  metadata.printData(queryOutput);

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
    size_t replicationBucketId = std::stoi(queryOutput.at("replicated_bucket_id").at(i)) - 1;

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

  auto queryOutput = getDataHelper(fileId, BucketType::PRIMARY);
  size_t lenghtOfColumn = queryOutput["chunk_key"].size();

  std::cout << "\nFetching Data..." << std::endl;

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
  std::cout << "Data fetched successfully" << std::endl;
  return result;
}

std::vector<std::unique_ptr<Chunk>> AwsS3::getBackupData(const size_t &fileId) {
  std::vector<std::unique_ptr<Chunk>> result;
  std::vector<std::future<std::unique_ptr<Chunk>>> activeThreads;

  auto queryOutput = getDataHelper(fileId, BucketType::REPLICATION);
  size_t lenghtOfColumn = queryOutput["chunk_key"].size();

  std::cout << "\nFetching Backup Data..." << std::endl;

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

  std::cout << "Backup Data fetched successfully" << std::endl;
  return result;
}

void AwsS3::updateMetaDataForDeletedFile(const BucketType &bucketType,
                                         const size_t &fileId) {
  std::string queryFile;
  if (bucketType == BucketType::PRIMARY) {
    queryFile = "DeleteFileQuery";
  } else if (bucketType == BucketType::REPLICATION) {
    queryFile = "DeleteTrashFile";
  }
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", queryFile)
    .singleData("id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  metadata.updateData(queryData);
}

void AwsS3::updateMetaDataFordeletedChunk(const BucketType &bucketType,
                                         const size_t &fileId) {
  std::string queryFile;
  if (bucketType == BucketType::PRIMARY) {
    queryFile = "DeleteChunkQuery";
  } else if (bucketType == BucketType::REPLICATION) {
    queryFile = "DeleteTrashChunkQuery";
  }
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", queryFile)
    .singleData("file_id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  metadata.updateData(queryData);
}

void AwsS3::addMetaDataToTrashFile(const size_t &fileId) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetFileDataQuery")
    .singleData("id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);

  std::string fileName = queryOutput.at("name").at(0);
  std::string fileType = queryOutput.at("type").at(0);

  jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "AddTrashFileQuery")
    .singleData("id", std::to_string(fileId))
    .singleData("name", fileName)
    .singleData("type", fileType)
    .getJsonData();

  auto queryDataTwo = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  metadata.updateData(queryDataTwo);
}

void AwsS3::addMetaDataToTrashChunk(const size_t &fileId) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetDataQuery")
    .singleData("file_id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);

  size_t lenghtOfColumn = queryOutput.at("chunk_key").size();

  for (size_t i = 0; i < lenghtOfColumn; i++) {
    std::string chunkKey = queryOutput.at("chunk_key").at(i);
    std::string bucketId = queryOutput.at("replicated_bucket_id").at(i);
    std::string objectKey = queryOutput.at("object_key").at(i);

    jsonData = Utils::SimpleJsonParser::JsonBuilder()
      .singleData("file", "AddTrashChunkQuery")
      .singleData("chunk_key", chunkKey)
      .singleData("replicated_bucket_id", bucketId)
      .singleData("object_key", objectKey)
      .singleData("file_id", std::to_string(fileId))
      .getJsonData();

    auto queryDataTwo = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
    metadata.updateData(queryDataTwo);
  }
}

void AwsS3::addMetaDataToTrash(const size_t &fileId) {
  addMetaDataToTrashFile(fileId);
  addMetaDataToTrashChunk(fileId);
}

void AwsS3::deleteDataHelper(MetaData::QueryResultMap &queryOutput,
                             int i,
                             const BucketType &bucketType) {
  std::string objectKey = queryOutput.at("object_key")[i];

  if (bucketType == BucketType::PRIMARY) {
    size_t primaryBucketId = std::stoi(queryOutput.at("primary_bucket_id")[i]) - 1;

    std::string primaryBucketName = this->buckets.at(primaryBucketId).first;
    std::string primaryBucketRegion = this->buckets.at(primaryBucketId).second;

    auto primaryBucketConfig = getConfig(primaryBucketRegion);
    AwsCrudOperHandler::deleteData(primaryBucketConfig, primaryBucketName, objectKey);
  } else if (bucketType == BucketType::REPLICATION) {
    size_t replicationBucketId = std::stoi(queryOutput.at("replicated_bucket_id")[i]) - 1;

    std::string replicationBucketName = this->buckets.at(replicationBucketId).first;
    std::string replicationBucketRegion = this->buckets.at(replicationBucketId).second;

    auto replicationBucketConfig = getConfig(replicationBucketRegion);
    AwsCrudOperHandler::deleteData(replicationBucketConfig, replicationBucketName, objectKey);
  }
}

void AwsS3::deleteData(const size_t &fileId) {
  std::vector<std::future<void>> activeThreads;

  auto queryOutput = getDataHelper(fileId, BucketType::PRIMARY);
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

  addMetaDataToTrash(fileId);
  updateMetaDataFordeletedChunk(BucketType::PRIMARY, fileId);
  updateMetaDataForDeletedFile(BucketType::PRIMARY, fileId);
}

void AwsS3::deleteBackupData(const size_t &fileId) {
  std::vector<std::future<void>> activeThreads;

  auto queryOutput = getDataHelper(fileId, BucketType::REPLICATION);
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
  updateMetaDataFordeletedChunk(BucketType::REPLICATION, fileId);
  updateMetaDataForDeletedFile(BucketType::REPLICATION, fileId);
}

std::vector<std::string> AwsS3::getChunkKeysFromTrash() {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetTrashChunkKey")
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);
  auto chunkKeys = queryOutput.at("chunk_key");
  return chunkKeys;
}

MetaData::QueryResultMap AwsS3::getFileFromTrash(const size_t &fileId) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetTrashFileName")
    .singleData("id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);
  return queryOutput;
}

void AwsS3::updateFileMetaDataForRestore(MetaData::QueryResultMap &queryResultMap) {
  std::string fileName = queryResultMap.at("name").at(0);
  std::string fileType = queryResultMap.at("type").at(0);

  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "InsertNewFile")
    .singleData("name", fileName)
    .singleData("type", fileType)
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(DATA_QUERIES_DIR, jsonData);
  metadata.updateData(queryData);
}

size_t AwsS3::getCurrentFileId() {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetFileId")
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(STORAGE_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);

  return std::stoi(queryOutput.at("id").at(0));
}

void AwsS3::restoreData(const size_t &fileId) {
  auto chunks = getBackupData(fileId);
  std::cout << "Reached" << std::endl;

  auto fileData = getFileFromTrash(fileId);
  updateFileMetaDataForRestore(fileData);

  size_t tempFileId = getCurrentFileId();
  auto chunkKeys = getChunkKeysFromTrash();

  std::vector<std::unique_ptr<Chunk>> tempChunks;

  int i = 0;

  for (const auto &chunk : chunks) {
    tempChunks.push_back(std::make_unique<Chunk>(
      chunk->getObjectKey(),
      chunkKeys.at(i),
      chunk->getEncryptedData(),
      tempFileId
    ));
    i++;
  }

  storeData(tempChunks);
  updateMetaDataFordeletedChunk(BucketType::REPLICATION, fileId);
  updateMetaDataForDeletedFile(BucketType::REPLICATION, fileId);
}

AwsS3::~AwsS3() {
  Aws::ShutdownAPI(options);
}
