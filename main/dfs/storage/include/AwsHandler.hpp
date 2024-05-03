#ifndef AWS_HANDLER_HPP
#define AWS_HANDLER_HPP

#include <aws/core/Aws.h>
#include "Storage.hpp"

class Storage::AwsHandler {
public:
  AwsHandler();
  ~AwsHandler();
private:
  void setupAwsConnection();
  Aws::SDKOptions options;
};
#endif // AWS_HANDLER_HPP
