#include "../include/DataChunker.hpp"
#include "../include/Content.hpp"
#include "../../utility/include/JsonStringBuilder.hpp"
#include "../../database/include/DatabaseHandler.hpp"
#include <cmath>

using namespace Data;

void DataChunker::storeFileMetaData(const Content &content) {
  Database::DatabaseHandler db;

  Utility::JsonStringBuilder builder;
  builder.singleData("file", "data/store_file")
         .singleData("name", content.getFileName())
         .singleData("type", content.getContentType())
         .build();

  db.updateData(builder);

  builder.clear();
  
  builder.singleData("file", "data/get_single_file_data")
         .singleData("id", "id")
         .singleData("name", "name")
         .singleData("value", content.getFileName())
         .build();

  auto data = db.getDataByRow(builder);
  this->fileId = data[1][0];
}

void DataChunker::setDefaultChunkRatio(int MainDataSize) {
  this->MainDataSize = MainDataSize;

  if (MainDataSize > 1000) {
    defaultChunkRatio = 0.1;
  } else if (MainDataSize > 100) {
    defaultChunkRatio = 0.2;
  } else if (MainDataSize > 10) {
    defaultChunkRatio = 0.25;
  } else if (MainDataSize > 5) {
    defaultChunkRatio = 0.5;
  } else {
    defaultChunkRatio = 1;
  }
}

size_t DataChunker::findChunkSize(const std::string &chunk) {
  size_t baseSize = sizeof(std::string);
  size_t memoryAllocationSize = chunk.capacity() * sizeof(char);
  return baseSize + memoryAllocationSize;
}

void DataChunker::setChunks() {
  int chunkNumber {1};
  int dataEndPos {0};
  int ratio = ceil(MainDataSize * defaultChunkRatio);

  while (dataEndPos < MainDataSize) {
    int dataStartPos {dataEndPos};
    if (dataEndPos + ratio > MainDataSize) {
      dataEndPos = MainDataSize;
    } else {
      dataEndPos += ratio;
    }
    std::string chunk = MainData.substr(dataStartPos, dataEndPos - dataStartPos);
    size_t chunkSize = findChunkSize(chunk);
    chunks.push_back(std::make_unique<Chunk>(fileId, chunk, chunkSize));
  }
}

const std::vector<std::unique_ptr<Chunk>> &DataChunker::getChunks() {
  return this->chunks;
}

void DataChunker::setMainData(const std::string &mainData, int contentLength) {
  this->MainData = mainData;
  setDefaultChunkRatio(contentLength);
}

DataChunker::DataChunker(const Content &content) {
  setMainData(content.getMainBody(), content.getContentLength());
  storeFileMetaData(content);
  setChunks();
}
