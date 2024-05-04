#include "../include/Handler.hpp"
#include "../include/DataChunker.hpp"
#include <string>

using namespace Data;

Handler::Handler() {
  this->awsHandler = std::make_unique<Storage::AwsHandler>();
}

void Handler::storeDataToStorage(Data::DataChunker &dataChunker) {
  auto chunks = dataChunker.getChunks();
  for (const auto &chunk : chunks) {
    int bucketNumber = std::stoi(chunk->getBucketNum());
    std::string objectKey = chunk->getObjectKey();
    std::string data = chunk->getChunkContent();

    awsHandler->storeData(bucketNumber, objectKey, data);
  }
}
