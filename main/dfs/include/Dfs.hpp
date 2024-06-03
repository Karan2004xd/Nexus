#ifndef DFS_HPP
#define DFS_HPP
#include "Nexus.hpp"
#include "DfsResult.hpp"
#include "../../data/include/Cache.hpp"
#include "../../user/include/User.hpp"

class Nexus::Dfs : protected Data::Cache, User {
protected:
  DfsResult storeDataApi(const std::string &fileName,
                         const std::string &fileContent,
                         const std::string &username);

  DfsResult getDataApi(const std::string &fileName,
                       const std::string &username);
  DfsResult getTrashDataApi(const std::string &fileName,
                            const std::string &username);

  DfsResult deleteDataApi(const std::string &fileName,
                          const std::string &username);
  DfsResult deleteTrashDataApi(const std::string &fileName,
                               const std::string &username);

  DfsResult listDataApi(const std::string &username);
  DfsResult listTrashDataApi(const std::string &username);

  DfsResult addUser(const std::string &username,
                    const std::string &password);
  DfsResult checkUser(const std::string &username,
                      const std::string &password);

  const bool checkIfVideoOrImage(const std::string &fileName) const;
  std::string base64Encode(const std::string &data);

private:
  enum FileType { NORMAL, TRASH };

  size_t getUserId(const std::string &username);

  size_t getFileId(const std::string &fileName,
                   const FileType &fileType,
                   const size_t &userId);

  size_t getTrashFileId(const std::string &fileName);

  std::string getCacheData(const size_t &fileId);
  DfsResult getDataApi(const std::string &fileName,
                       const FileType &fileType,
                       const std::string &username);

  DfsResult deleteDataApi(const std::string &fileName,
                          const FileType &fileType,
                          const std::string &username);

  DfsResult listDataApi(const FileType &fileType, const std::string &username);
};
#endif // DFS_HPP
