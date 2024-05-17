#ifndef CHUNKER_HPP
#define CHUNKER_HPP
#include "../../dfs/include/Nexus.hpp"
#include "Chunk.hpp"

using namespace Nexus;

class Data::Chunker {
public:
  Chunker() {};
  Chunker(const Contents &contents);

  const std::vector<std::unique_ptr<Chunk>> &getChunks();
  const std::vector<std::unique_ptr<Chunk>> &getChunks(const Contents &contents);
private:
  MetaData metadata;
  std::vector<std::unique_ptr<Chunk>> chunks;

  size_t fileSize;
  size_t fileId;

  const int MAX_CHUNKS = 4;

  void setFileSize(const size_t &fileSize) { this->fileSize = fileSize; }
  void setChunks(const std::string &fileData);

  void setNewFileInMetadata(const std::string &fileName,
                            const std::string &fileType);

  void setFileId();
  void setupChunker(const Contents &contents);
};
#endif // CHUNKER_HPP
