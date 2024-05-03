#include "../include/AwsHandler.hpp"
#include "../../../constants.h"

#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>
#include <string>

using namespace Storage;
using namespace Aws;
using namespace Aws::Auth;

void AwsHandler::setupAwsConnection() {
  Aws::InitAPI(options);
  Aws::Auth::AWSCredentials credentials {std::getenv(std::string(ACCESS_KEY_ID).c_str()),
                                         std::getenv(std::string(SECRET_ACCESS_KEY).c_str())};
  Aws::S3::S3Client s3Client {credentials};
  auto outCome = s3Client.ListBuckets();

  if (outCome.IsSuccess()) {
    std::cout << "Connection established" << std::endl;
  } else {
    throw std::runtime_error("Error in AwsHandler : Connection cannot be established");
  }
}

AwsHandler::AwsHandler() {
  setupAwsConnection();
}

AwsHandler::~AwsHandler() {
  Aws::ShutdownAPI(options);
}
