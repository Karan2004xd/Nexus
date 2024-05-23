#ifndef CACHE_HPP
#define CACHE_HPP
#include "Chunk.hpp"

using namespace Nexus;

class Data::Cache { 
public:
  typedef std::vector<std::unique_ptr<Chunk>> ChunkedData;

  Cache();

  virtual void storeData(const ChunkedData &chunks);
  virtual ChunkedData getData(const size_t &fileId);
  virtual void deleteData(const size_t &fileId);

  virtual ~Cache() = default;
private:
  enum FileType {NORMAL, TRASH};

  std::string getDirName(const size_t &fileId, const FileType &fileType);
  std::string getDirName(const std::string &path,
                         const std::string &name = {});

  bool checkDirectory(const std::string &path,
                      const std::string &name = {});
  bool checkFile(const std::string &path,
                 const std::string &name = {});

  void createDirectory(const std::string &path,
                       const std::string &dirName = {});
  bool checkIfDirectoryisEmpty(const std::string &path);
  
  // Store Data Helpers
  void createFile(const std::string &path,
                  const std::string &name = {});
  void makeFilesWriteProtected(const std::string &path,
                               const std::string &name = {});

  void writeDataIntoFile(const std::string &path,
                         const std::string &name,
                         const std::string &fileContent);
  size_t getFileId(const std::string &objectKey);
  std::string getFileName(const size_t &fileId, const FileType &fileType);

  // Get Data Helpers
  // (objectKey, ChunkId)
  typedef std::unordered_map<std::string, std::string> ChunkKeys;
  size_t getNumberOfFilesInDir(const std::string &dirPath);
  size_t getNumberOfChunksForFile(const size_t &fileId);
  std::string readDataFromFile(const std::string &dirPath,
                               const std::string &fileName);
  ChunkKeys getChunkIdsFromMetaData(const size_t &fileId);
};

#endif // CACHE_HPP
