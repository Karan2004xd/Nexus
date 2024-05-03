#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/sts/STSClient.h>
#include <aws/s3/S3Client.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/auth/STSCredentialsProvider.h>

#include "../../main/variables.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace Aws;
using namespace Aws::Auth;

int main() {
  Aws::SDKOptions options;
  Aws::InitAPI(options);
  int result = 0;

  {
    Aws::Auth::AWSCredentials credentials {std::getenv("ACCESS_KEY_ID"), std::getenv("SECRET_ACCESS_KEY")};
    Aws::S3::S3Client s3Client {credentials};
    auto outcome = s3Client.ListBuckets();

    if (!outcome.IsSuccess()) {
      std::cerr << "Failed with error: " << outcome.GetError() << std::endl;
      result = 1;
    } else {
      std::cout << "Found " << outcome.GetResult().GetBuckets().size()
                << " buckets\n";

      for (const auto &bucket : outcome.GetResult().GetBuckets()) {
        std::cout << bucket.GetName() << std::endl;
      }
    }
  }
  Aws::ShutdownAPI(options);
  return result;
}

