#include "../include/Handler.hpp"
#include "../include/DataChunker.hpp"

#include <string>

using namespace Data;

Handler::Handler() {
  this->awsHandler = std::make_unique<Storage::AwsHandler>();
  this->builder = std::make_unique<Utility::JsonStringBuilder>();
  this->db = std::make_unique<Database::DatabaseHandler>();
}

std::string Handler::getFileName(const std::string &objectKey) {
  std::string fileName;
  size_t pos = objectKey.find('_');
  fileName = objectKey.substr(0, pos);
  return fileName;
}

void Handler::deleteFileMetaData(const std::string &fileName) {
  builder->clear();

  builder->singleData("file", "data/delete_file")
    .singleData("value", fileName)
    .build();

  db->updateData(builder);
}

void Handler::deleteChunkMetaData(const std::string &objectKey) {
  builder->clear();

  builder->singleData("file", "data/delete_chunk")
    .singleData("value", objectKey)
    .build();

  db->updateData(builder);
}

void Handler::storeDataToStorage(Data::DataChunker &dataChunker) {
  bool deleteFile = false;
  std::string fileName;

  for (const auto &chunk : dataChunker.getChunks()) {
    int bucketNumber = std::stoi(chunk->getBucketNum());
    std::string objectKey = chunk->getObjectKey();
    std::string data = chunk->getChunkContent();

    try {
      awsHandler->storeData(bucketNumber, objectKey, data);
    } catch (std::exception &) {
      deleteFile = true;
      fileName = getFileName(objectKey);
      deleteChunkMetaData(objectKey);
    }
  }

  if (deleteFile) {
    deleteFileMetaData(fileName);
  }
}

void Handler::deleteDataFromStorage(const std::string &fileName) {
  builder->clear();
  builder->singleData("file", "data/delete_data_from_storage")
          .singleData("name", fileName)
          .build();

  auto data = db->getDataByRow(builder);
  if (data.size() > 0) {
    std::string fileName;
    for (const auto &rowData : data) {
      int bucketNumber = std::stoi(rowData.second[0]);
      std::string objectKey = rowData.second[1];

      awsHandler->deleteData(bucketNumber, objectKey);
      deleteChunkMetaData(objectKey);
      fileName = getFileName(objectKey);
    }
    deleteFileMetaData(fileName);
  } else {
    std::cerr << "Data from the query is empty" << std::endl;
    throw std::runtime_error("Error in Handler (deleteDataFromStorage)");
  }
}

void Handler::getDataFromStorge(const std::string &fileName) {
  builder->clear();
  builder->singleData("file", "data/get_data_from_storage")
          .singleData("name", fileName)
          .build();

  auto data = db->getDataByRow(builder);
  if (data.size() > 0) {
    std::vector<std::unique_ptr<Chunk>> chunks;
    for (const auto &rowData : data) {
      int bucketNumber = std::stoi(rowData.second[0]);
      std::string chunkKey = rowData.second[1];
      std::string objectKey = rowData.second[2];
      
      std::string chunkData = awsHandler->getData(bucketNumber, objectKey);
      chunks.push_back(std::make_unique<Chunk>(chunkData, chunkKey));
    }
  } else {
    std::cerr << "Data from the query is empty" << std::endl;
    throw std::runtime_error("Error in Handler (getDataFromStorge)");
  }
}
