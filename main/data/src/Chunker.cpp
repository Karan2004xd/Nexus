#include "../include/Chunker.hpp"
#include "../include/Contents.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"

using namespace Data;

void Chunker::setChunks(const std::string &fileData) {
  size_t chunkSize = this->fileSize / MAX_CHUNKS;
  size_t dataEndPos = 0;

  while (dataEndPos < this->fileSize) {
    size_t dataStartPos {dataEndPos};
    if (dataEndPos + chunkSize > this->fileSize) {
      dataEndPos = this->fileSize;
    } else {
      dataEndPos += chunkSize;
    }

    std::string chunkContent = fileData.substr(dataStartPos,
                                               dataEndPos - dataStartPos);
    chunks.push_back(std::make_unique<Chunk>(chunkContent, this->fileId));
  }
}

void Chunker::setNewFileInMetadata(const std::string &fileName,
                                   const std::string &fileType) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "InsertNewFile")
    .singleData("name", fileName)
    .singleData("type", fileType)
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(DATA_QUERIES_DIR, jsonData);

  metadata.updateData(queryData);
}

void Chunker::setFileId() {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetFileIds")
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(DATA_QUERIES_DIR, jsonData);
  auto queryOutput = metadata.getQueryDataMap(queryData);
  auto ids = queryOutput["id"];
  this->fileId = std::stoi(ids[ids.size() - 1]);
}

void Chunker::setupChunker(const Contents &contents) {
  setNewFileInMetadata(contents.getFileName(), contents.getFileType());
  setFileId();
  setFileSize(contents.getfileLength());
  setChunks(contents.getFileContent());
}

const std::vector<std::unique_ptr<Chunk>> *Chunker::getChunks() {
  return &this->chunks;
}

const std::vector<std::unique_ptr<Chunk>> *Chunker::getChunks(const Contents &contents) {
  setupChunker(contents);
  return &this->chunks;
}

Chunker::Chunker(const Contents &contents) {
  setupChunker(contents);
}
