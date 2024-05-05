#include "../include/Handler.hpp"
#include "../include/DataChunker.hpp"
#include <string>

using namespace Data;

Handler::Handler() {
  this->awsHandler = std::make_unique<Storage::AwsHandler>();
}

void Handler::storeDataToStorage(Data::DataChunker &dataChunker) {
  for (const auto &chunk : dataChunker.getChunks()) {
    int bucketNumber = std::stoi(chunk->getBucketNum());
    std::string objectKey = chunk->getObjectKey();
    std::string data = chunk->getChunkContent();

    awsHandler->storeData(bucketNumber, objectKey, data);
  }
}
