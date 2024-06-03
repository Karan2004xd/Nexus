#ifndef DFS_HPP
#define DFS_HPP
#include "Nexus.hpp"
#include "DfsResult.hpp"
#include "../../data/include/Cache.hpp"

class Nexus::Dfs : protected Data::Cache {
protected:
  DfsResult storeDataApi(const std::string &fileName,
                         const std::string &fileContent);

  DfsResult getDataApi(const std::string &fileName);
  DfsResult getTrashDataApi(const std::string &fileName);

  DfsResult deleteDataApi(const std::string &fileName);
  DfsResult deleteTrashDataApi(const std::string &fileName);

  DfsResult listDataApi();
  DfsResult listTrashDataApi();

private:
  enum FileType { NORMAL, TRASH };

  size_t getFileId(const std::string &fileName,
                   const FileType &fileType);
  size_t getTrashFileId(const std::string &fileName);

  const bool checkIfVideoOrImage(const std::string &fileName) const;
  std::string base64Encode(const std::string &data);

  std::string getCacheData(const size_t &fileId);
  DfsResult getDataApi(const std::string &fileName,
                       const FileType &fileType);

  DfsResult deleteDataApi(const std::string &fileName,
                          const FileType &fileType);

  DfsResult listDataApi(const FileType &fileType);
};
#endif // DFS_HPP
