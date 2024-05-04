#ifndef HANDLER_HPP
#define HANDLER_HPP
#include "../../storage/include/AwsHandler.hpp"

#include "Data.hpp"
#include <memory>

class Data::Handler {
public:
  Handler();
  void storeDataToStorage(Data::DataChunker &);
private:
  std::unique_ptr<Storage::AwsHandler> awsHandler;
};
#endif // HANDLER_HPP
