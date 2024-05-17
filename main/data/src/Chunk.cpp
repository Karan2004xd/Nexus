#include "../include/Chunk.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"
#include <iostream>
#include <sstream>

using namespace Nexus;

const std::string Chunk::getDecryptedData() {
  return decryptData(this->data.first, this->data.second);
}

void Chunk::setRawData(const std::string &fileName) {
  if (!fileName.empty()) {
    this->data = DataEncryptor::encryptData(fileName);
  }
}

void Chunk::setChunkId() {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetChunkIds")
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(DATA_QUERIES_DIR, jsonData);

  auto queryOutput = metaData->getQueryDataMap(queryData);
  if (queryOutput.size() > 0) {
    auto ids = queryOutput.at("id");
    this->chunkId = std::stoi(ids[ids.size() - 1]);
  } else {
    this->chunkId = 0;
  }
  this->chunkId += 1;
}

void Chunk::setObjectKey() {
  std::string chunkKey = this->data.first;
  std::ostringstream oss;
  oss << this->chunkId << "_" << this->fileId << "_"
      << chunkKey.substr(0, 10);

  this->objectKey = oss.str();
}

void Chunk::updateMetadata() {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "InsertWithoutBucketInfo")
    .singleData("chunk_key", this->data.first)
    .singleData("object_key", this->objectKey)
    .singleData("file_id", std::to_string(this->fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(DATA_QUERIES_DIR, jsonData);
  /* std::cout << queryData.getParsedData() << std::endl; */
  metaData->updateData(queryData);
}

void Chunk::setupChunk(const std::string &rawChunkData,
                       const size_t &fileId) {
  setRawData(rawChunkData);
  setFileId(fileId);
  setChunkId();
  setObjectKey();
  updateMetadata();
}

Chunk::Chunk(const std::string &rawchunk, const size_t &fileId) {
  this->metaData = std::make_unique<MetaData>();
  setupChunk(rawchunk, fileId);
}
