#ifndef AWS_S3_HPP
#define AWS_S3_HPP
#include "AwsCrudOperHandler.hpp"
#include "AwsS3SetupHandler.hpp"
#include "StorageBase.hpp"

using namespace Nexus;

class Storage::AwsS3 : protected StorageBase, AwsCrudOperHandler,
                                 AwsS3SetupHandler {
protected:
  AwsS3();

  void storeData(const std::vector<std::unique_ptr<Chunk>> &chunks) override;

  std::vector<std::unique_ptr<Chunk>> getData(const size_t &fileId) override;
  std::vector<std::unique_ptr<Chunk>> getBackupData(const size_t &fileId) override;

  void deleteData(const size_t &fileId) override;
  /* void restoreData(const size_t &filedId) override; */

  void deleteBackupData(const size_t &fileId) override;

  ~AwsS3();

private:
  enum BucketType { PRIMARY, REPLICATION };
  Aws::SDKOptions options;
  
  // Store Helper
  Aws::S3::S3ClientConfiguration getConfig(const std::string &region);
  void storeDataHelper(const std::string &objectKey,
                       const std::string &data);

  // Get Helper
  MetaData::QueryResultMap getDataHelper(const size_t &fileId, const BucketType &bucketType);
  std::unique_ptr<Chunk> getDataHelper2(MetaData::QueryResultMap &queryResultMap,
                                        int columnNum,
                                        const BucketType &bucketType);

  // Delete Helper
  void deleteDataHelper(MetaData::QueryResultMap &queryResultMap,
                        int columnNum,
                        const BucketType &bucketType);
  void updateMetaDataForDeletedFile(const BucketType &bucketType,
                                    const size_t &fileId);
  void updateMetaDataFordeletedChunk(const BucketType &bucketType,
                                     const size_t &fileId);

  void addMetaDataToTrashFile(const size_t &filedId);
  void addMetaDataToTrashChunk(const size_t &filedId);
  void addMetaDataToTrash(const size_t &fileId);

  // Restore Helper
  /* std::vector<std::string> getChunkKeysFromTrash(); */
  /* MetaData::QueryResultMap getFileFromTrash(const size_t &fileId); */

  /* void updateFileMetaDataForRestore(MetaData::QueryResultMap &queryResultMap); */
  /* size_t getCurrentFileId(); */
};
#endif // AWS_S3_HPP
