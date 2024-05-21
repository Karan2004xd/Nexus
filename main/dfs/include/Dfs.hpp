#ifndef DFS_HPP
#define DFS_HPP
#include "Nexus.hpp"
#include "DfsResult.hpp"

class Nexus::Dfs {
protected:
  DfsResult storeDataApi(const std::string &fileName,
                         const std::string &fileContent);

  DfsResult getDataApi(const std::string &fileName);
  DfsResult getTrashDataApi(const std::string &fileName);

  DfsResult deleteDataApi(const std::string &fileName);
  DfsResult deteteTrashDataApi(const std::string &fileName);
};
#endif // DFS_HPP
