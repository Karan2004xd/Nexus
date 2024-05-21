#ifndef DFS_HPP
#define DFS_HPP
#include "Nexus.hpp"
#include "DfsResult.hpp"
#include "../../storage/include/AwsS3.hpp"
#include "../../data/include/Cache.hpp"

class Nexus::Dfs : protected Storage::AwsS3, Data::Cache {
public:
  DfsResult storeDataApi(const std::string &fileName,
                         const std::string &fileContent);

  DfsResult getDataApi(const std::string &fileName);
  DfsResult getTrashDataApi(const std::string &fileName);

  DfsResult deleteDataApi(const std::string &fileName);
  DfsResult deteteTrashDataApi(const std::string &fileName);

private:
  enum FileType { NORMAL, TRASH };
  MetaData metaData;

  size_t getFileId(const std::string &fileName,
                   const FileType &fileType);
  size_t getTrashFileId(const std::string &fileName);

  std::string getCacheData(const size_t &fileId);
  DfsResult getDataApi(const std::string &fileName,
                       const FileType &fileType);

  DfsResult deleteDataApi(const std::string &fileName,
                          const FileType &fileType);
};
#endif // DFS_HPP
