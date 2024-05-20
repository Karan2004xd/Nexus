#include "../include/Cache.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"

namespace fs = std::filesystem;
using namespace Data;

Cache::Cache() {
  if (!checkDirectory(CACHE_DIR)) {
    createDirectory(CACHE_DIR);
  }
}

std::string Cache::getDirName(const std::string &path,
                              const std::string &name) {
  std::string dir = path;
  if (!name.empty()) {
    dir += "/" + name;
  }
  return dir;
}

std::string Cache::getDirName(const size_t &fileId) {
  std::string tempFileName = getFileName(fileId);
  std::string fileName = tempFileName.substr(0, tempFileName.find("."));

  std::string dir = getDirName(CACHE_DIR, fileName);
  return dir;
}

bool Cache::checkDirectory(const std::string &path,
                           const std::string &name) {
  std::string dir = getDirName(path, name);
  return fs::exists(dir) && fs::is_directory(dir);
}

void Cache::createDirectory(const std::string &path,
                            const std::string &dirName) {
  std::string dir = getDirName(path, dirName);
  if (fs::create_directory(dir)) {
    std::cout << "Created Directory : " << dir << std::endl;
  } else {
    std::cerr << "Error while creating a new directory : " << dir << std::endl;
    throw std::runtime_error("(Cache) : Error in checkAndCreateDirectory");
  }
}

bool Cache::checkFile(const std::string &path,
                      const std::string &name) {
  std::string dir = getDirName(path, name);
  return fs::exists(dir) && fs::is_regular_file(dir);
}

void Cache::createFile(const std::string &path,
                       const std::string &name) {
  std::string dir = getDirName(path, name) + EXT;

  std::ofstream outFile {dir};
  if (outFile.is_open()) {
    std::cout << "File Created at: " << dir << std::endl;
    outFile.close();
  } else {
    std::cerr << "Failed to create file at : " << dir << std::endl;
    throw std::runtime_error("(Cache) : Error in createFile");
  }
}

void Cache::writeDataIntoFile(const std::string &path,
                              const std::string &name,
                              const std::string &fileContent) {
  std::string dir = getDirName(path, name) + EXT;
  std::ofstream file {dir};
  if (file.is_open()) {
    file << fileContent;
  }
  file.close();
}

void Cache::makeFilesWriteProtected(const std::string &path,
                                    const std::string &name) {
  std::string dir = getDirName(path, name);
  if (checkDirectory(dir)) {
    for (const auto &entry : fs::directory_iterator(dir)) {
      if (fs::is_regular_file(entry.path())) {
        fs::permissions(entry.path(),
                        fs::perms::owner_write | fs::perms::others_write | fs::perms::group_write,
                        fs::perm_options::remove);
        std::cout << "Made write protected: " << entry.path() << std::endl;
      } else {
        throw std::runtime_error("(Cache) : File not found");
      }
    }
  } else {
    throw std::runtime_error("(Cache) : Directory not found");
  }
}

size_t Cache::getFileId(const std::string &objectKey) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetFileIdFromChunk")
    .singleData("object_key", objectKey)
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(CACHE_QUERIES_DIR, jsonData);
  auto queryOutput = metaData.getQueryDataMap(queryData);
  return std::stoi(queryOutput.at("file_id").at(0));
}

std::string Cache::getFileName(const size_t &fileId) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetFileName")
    .singleData("id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(CACHE_QUERIES_DIR, jsonData);
  auto queryOutput = metaData.getQueryDataMap(queryData);
  return queryOutput.at("name").at(0);
}

bool Cache::checkIfDirectoryisEmpty(const std::string &path) {
  return fs::directory_iterator(path) == fs::directory_iterator();
}

void Cache::storeData(const ChunkedData &chunks) {
  size_t fileId = getFileId(chunks.at(0)->getObjectKey());

  std::string dir = getDirName(fileId);

  if (!checkDirectory(dir) || checkIfDirectoryisEmpty(dir)) {
    if (!checkDirectory(dir)) {
      createDirectory(dir);
    }

    for (const auto &chunk : chunks) {
      createFile(dir, chunk->getObjectKey());
      writeDataIntoFile(dir, chunk->getObjectKey(), chunk->getEncryptedData());
    }
    makeFilesWriteProtected(dir);
  }
}

void Cache::deleteData(const size_t &fileId) {
  std::string dir = getDirName(fileId);

  if (checkDirectory(dir)) {
    std::uintmax_t n = fs::remove_all(dir);
    std::cout << "Removed " << n << " files from : " << dir << std::endl;
    std::cout << "Removed directory from : " << dir << std::endl;
  }
}

size_t Cache::getNumberOfFilesInDir(const std::string &dirPath) {
  size_t result {0};
  for (const auto &entry : fs::directory_iterator(dirPath)) {
    if (fs::is_regular_file(entry.path())) {
      result++;
    }
  }
  return result;
}

size_t Cache::getNumberOfChunksForFile(const size_t &fileId) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetChunkCount")
    .singleData("file_id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(CACHE_QUERIES_DIR, jsonData);
  auto queryOutput = metaData.getQueryDataMap(queryData);
  return std::stoi(queryOutput.at("count").at(0));
}

std::string Cache::readDataFromFile(const std::string &dirPath,
                                    const std::string &fileName) {
  std::ostringstream oss;
  std::string dir = getDirName(dirPath, fileName);
  if (checkFile(dir)) {
    std::ifstream file {dir};
    if (file.is_open()) {
      oss << file.rdbuf();
      file.close();
    } else {
      throw std::runtime_error("(Cache) : File cannot be opened at " + dir);
    }
  } else {
    throw std::runtime_error("(Cache) : File not found at " + dir);
  }
  return oss.str();
}

Cache::ChunkKeys Cache::getChunkIdsFromMetaData(const size_t &fileId) {
  ChunkKeys chunkKeys;
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "GetChunkKeys")
    .singleData("file_id", std::to_string(fileId))
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(CACHE_QUERIES_DIR, jsonData);
  auto queryOutput = metaData.getQueryDataMap(queryData);
  
  int lengthOfColumn = queryOutput.at("chunk_key").size();
  for (int i = 0; i < lengthOfColumn; i++) {
    std::string objectKey = queryOutput.at("object_key").at(i);
    std::string chunkKey = queryOutput.at("chunk_key").at(i);
    chunkKeys.insert({objectKey, chunkKey});
  }
  return chunkKeys;
}

Cache::ChunkedData Cache::getData(const size_t &fileId) {
  ChunkedData chunks;
  std::string dir = getDirName(fileId);

  if (checkDirectory(dir)) {
    if (getNumberOfFilesInDir(dir) == getNumberOfChunksForFile(fileId)) {
      auto chunkKeys = getChunkIdsFromMetaData(fileId);

      for (const auto &key : chunkKeys) {
        std::string data = readDataFromFile(dir, key.first + EXT);
        chunks.push_back(std::make_unique<Chunk>(data, key.second));
      }

    } else {
      std::cerr << "(Cache) : Corrupted cache data at " << dir << std::endl;
      std::cerr << "Removing the corrupted data" << std::endl;
      throw std::runtime_error("(Cache): Corrupted data");
    }
  } else {
    throw std::runtime_error("(Cache) : No directory found at " + dir);
  }
  std::reverse(chunks.begin(), chunks.end());
  return chunks;
}
