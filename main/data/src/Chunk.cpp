#include "../include/Chunk.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"
#include <iostream>
#include <sstream>

using namespace Nexus;

Chunk::Chunk(const Chunk &other) {
  this->chunkId = other.chunkId;
  this->fileId = other.fileId;
  this->data = other.data;
  this->objectKey = other.objectKey;
}

std::unique_ptr<Chunk> Chunk::clone() {
  return std::make_unique<Chunk>(*this);
}

const std::string Chunk::getDecryptedData() {
  return decryptData(this->data.first, this->data.second);
}

const std::string Chunk::getEncryptedData() {
  return this->data.second;
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

  auto queryOutput = metaData.getQueryDataMap(queryData);
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
      << chunkKey.substr(0, DEFAULT_OBJECT_KEY_SIZE);

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
  metaData.updateData(queryData);
}

std::string Chunk::getObjectKeyUsingChunkKey(const std::string &chunkKey,
                                             const std::string &queryFileName) {
  std::string fileName;
  if (queryFileName.empty()) {
    fileName = "GetObjectKeyFromChunk";
  } else {
    fileName = queryFileName;
  }

  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", fileName)
    .singleData("chunk_key", chunkKey)
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(DATA_QUERIES_DIR, jsonData);

  auto queryOutput = metaData.getQueryDataMap(queryData);
  std::string result;

  try {
    result = queryOutput.at("object_key").at(0);
  } catch (const std::exception &) {
    result = getObjectKeyUsingChunkKey(chunkKey, "GetObjectKeyFromTrash");
  }
  return result;
}

size_t Chunk::getFileIdUsingChunkKey(const std::string &chunkKey) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetFileIdFromChunk")
    .singleData("chunk_key", chunkKey)
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(DATA_QUERIES_DIR, jsonData);
  auto queryOutput = metaData.getQueryDataMap(queryData);
  return std::stoi(queryOutput.at("file_id").at(0));
}

void Chunk::setupChunk(const std::string &rawChunkData,
                       const size_t &fileId) {
  setRawData(rawChunkData);
  setFileId(fileId);
  setChunkId();
  setObjectKey();
  updateMetadata();
}

void Chunk::setData(const std::string &encryptedData,
                    const std::string &chunkKey) {
  this->data = std::make_pair(chunkKey, encryptedData);
}

Chunk::Chunk(const std::string &rawchunk, const size_t &fileId) {
  setupChunk(rawchunk, fileId);
}

Chunk::Chunk(const std::string &encryptedData, const std::string &chunkKey) {
  setData(encryptedData, chunkKey);
  setObjectKey(getObjectKeyUsingChunkKey(chunkKey));
}

Chunk::Chunk(const std::string &objectKey,
             const std::string &chunkKey,
             const std::string &data,
             const size_t &fileId) {
  this->objectKey = objectKey;

  setData(data, chunkKey);
  setFileId(fileId);

  setChunkId();
  setObjectKey();

  updateMetadata();
}
