#include "../include/Dfs.hpp"
#include "../../data/include/Contents.hpp"
#include "../../data/include/Chunker.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"

using namespace Nexus;

DfsResult Dfs::storeDataApi(const std::string &fileName, const std::string &fileContent) { std::string output, errorMsg;
  DfsResult::ResultType resultType = DfsResult::ResultType::SUCCESS;

  try {
    Data::Contents contents {fileName, fileContent};
    Data::Chunker chunker {contents};
    auto &chunks = chunker.getChunks();

    Storage::AwsS3::storeData(chunks);
  } catch (std::exception &e) {
    errorMsg = e.what();
    output = "Failed to create new file";
    resultType = DfsResult::ResultType::FAILED;
  }

  return {output, errorMsg, resultType};
}

size_t Dfs::getFileId(const std::string &fileName,
                      const FileType &fileType) {
  std::string queryFileName;

  if (fileType == FileType::NORMAL) {
    queryFileName = "GetFileIdFromName";
  } else if (fileType == FileType::TRASH) {
    queryFileName = "GetTrashFileIdFromName";
  }
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", queryFileName)
    .singleData("name", fileName)
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(DFS_QUERIES_DIR, jsonData);
  MetaData metaData;
  auto queryOutput = metaData.getQueryDataMap(queryData);
  return std::stoi(queryOutput.at("id").at(0));
}

std::string Dfs::getCacheData(const size_t &fileId) {
  std::string output;
  try {
    auto chunks = Data::Cache::getData(fileId);
    std::cout << "Yes 3" << std::endl;
    for (const auto &chunk : chunks) {
      output += chunk->getDecryptedData();
    }
  } catch (std::exception &e) {
    throw std::runtime_error(e.what());
  }
  return output;
}

DfsResult Dfs::getDataApi(const std::string &fileName,
                          const FileType &fileType) {
  std::string output, errorMsg;
  DfsResult::ResultType resultType = DfsResult::ResultType::SUCCESS;
  std::future<void> cacheThread;

  try {
    size_t fileId = getFileId(fileName, fileType);

    try {
      output = getCacheData(fileId);
    } catch (const std::exception &) {
      if (fileType == FileType::NORMAL) {
        auto chunks = Storage::AwsS3::getData(fileId);

        for (const auto &chunk : chunks) {
          output += chunk->getDecryptedData();
        }
        Data::Cache::storeData(chunks);
      } else if (fileType == FileType::TRASH) {
        auto chunks = Storage::AwsS3::getBackupData(fileId);

        for (const auto &chunk : chunks) {
          output += chunk->getDecryptedData();
        }
      }
    }
  } catch (std::exception &e) {
    errorMsg = e.what();
    output = "Failed to fetch data";
    resultType = DfsResult::ResultType::FAILED;
  }

  return {output, errorMsg, resultType};
}

DfsResult Dfs::getDataApi(const std::string &fileName) {
  return getDataApi(fileName, FileType::NORMAL);
}

DfsResult Dfs::getTrashDataApi(const std::string &fileName) {
  return getDataApi(fileName, FileType::TRASH);
}

DfsResult Dfs::deleteDataApi(const std::string &fileName,
                             const FileType &fileType) {
  std::string output, errorMsg;
  DfsResult::ResultType resultType = DfsResult::ResultType::SUCCESS;

  try {
    size_t fileId = getFileId(fileName, fileType);
    if (fileType == FileType::NORMAL) {
      Storage::AwsS3::deleteData(fileId);
    } else if (fileType == FileType::TRASH) {
      Data::Cache::deleteData(fileId);
      Storage::AwsS3::deleteBackupData(fileId);
    }

  } catch (std::exception &e) {
    errorMsg = e.what();
    output = "Unable to delete the provided data";
    resultType = DfsResult::ResultType::FAILED;
  }
  return {output, errorMsg, resultType};
}

DfsResult Dfs::deleteDataApi(const std::string &fileName) {
  return deleteDataApi(fileName, FileType::NORMAL);
}

DfsResult Dfs::deleteTrashDataApi(const std::string &fileName) {
  return deleteDataApi(fileName, FileType::TRASH);
}

DfsResult Dfs::listDataApi(const FileType &fileType) {
  DfsResult::MessageMap messageMap;

  std::string output, errorMsg;
  DfsResult::ResultType resultType = DfsResult::ResultType::SUCCESS;

  try {
    std::string queryFile;
    if (fileType == FileType::NORMAL) {
      queryFile = "ListFileData";
    } else if (fileType == FileType::TRASH) {
      queryFile = "ListTrashFileData";
    }
    auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
      .singleData("file", queryFile)
      .getJsonData();

    auto queryData = Utils::SimpleQueryParser::parseQuery(DFS_QUERIES_DIR, jsonData);
    auto queryOutput = metaData.getQueryDataMap(queryData);
    
    for (const auto &key : queryOutput) {
      messageMap.insert({key.first, key.second});
    }
  } catch (const std::exception &e) {
    errorMsg = e.what();
    output = "Failed to fetch the data";
    resultType = DfsResult::ResultType::FAILED;
    return {output, errorMsg, resultType};
  }
  return {messageMap, errorMsg, resultType};
}

DfsResult Dfs::listDataApi() {
  return listDataApi(FileType::NORMAL);
}

DfsResult Dfs::listTrashDataApi() {
  return listDataApi(FileType::TRASH);
}
