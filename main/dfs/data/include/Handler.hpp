#ifndef HANDLER_HPP
#define HANDLER_HPP
#include "../../storage/include/AwsHandler.hpp"
#include "../../utility/include/JsonStringBuilder.hpp"
#include "../../database/include/DatabaseHandler.hpp"

#include "Chunk.hpp"
#include "Data.hpp"
#include <memory>

class Data::Handler {
public:
  Handler();
  void storeDataToStorage(Data::DataChunker &);
  void deleteDataFromStorage(const std::string &);
  std::string getDataFromStorge(const std::string &);
private:
  std::unique_ptr<Storage::AwsHandler> awsHandler;
  std::unique_ptr<Database::DatabaseHandler> db;
  std::unique_ptr<Utility::JsonStringBuilder> builder;

  std::string getFileName(const std::string &);
  void deleteChunkMetaData(const std::string &);
  void deleteFileMetaData(const std::string &);

  std::string getChunksData(std::vector<std::unique_ptr<Chunk>> &);
};
#endif // HANDLER_HPP
