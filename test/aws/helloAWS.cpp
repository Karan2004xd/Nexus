#include <aws/core/Aws.h>
#include <aws/core/Region.h>

#include <aws/core/client/ClientConfiguration.h>
#include <aws/s3/S3ClientConfiguration.h>

#include <aws/sts/STSClient.h>
#include <aws/s3/S3Client.h>

#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>

#include <iostream>
#include <sstream>

using namespace Aws;
using namespace Aws::Auth;

const char *bucketName = "nex-test-bucket";
const char *objectKey = "testObjKey";

// Use Aws cli to configure connection
// use command : aws configure 
// and then enter your access key and secret access id

void storeData(const std::string &data, const Aws::S3::S3Client &s3Client) {
  Aws::S3::Model::PutObjectRequest objRequest;
  objRequest.SetBucket(bucketName);
  objRequest.SetKey(objectKey);

  auto dataStream = Aws::MakeShared<Aws::StringStream>("Data");
  *dataStream << data;
  objRequest.SetBody(dataStream);


  auto putObjectOutcome = s3Client.PutObject(objRequest);
  if (putObjectOutcome.IsSuccess()) {
    std::cout << "Object uploaded successfully to S3" << std::endl;
  } else {
    std::cerr << "Failed to upload object to S3: " <<  putObjectOutcome.GetError() << std::endl;
  }
}

void deleteData(const Aws::S3::S3Client &s3Client) {
  Aws::S3::Model::DeleteObjectRequest request;
  request.WithKey(objectKey).WithBucket(bucketName);
  Aws::S3::Model::DeleteObjectOutcome outcome = s3Client.DeleteObject(request);

  if (outcome.IsSuccess()) {
    std::cout << "Successfully deleted object" << std::endl;
  } else {
    std::cerr << "Failed to delete the object: " << outcome.GetError() << std::endl;
  }
}

void getData(const Aws::S3::S3Client &s3Client) {
  std::stringstream oss;
  Aws::S3::Model::GetObjectRequest request;
  request.SetBucket(bucketName);
  request.SetKey(objectKey);

  Aws::S3::Model::GetObjectOutcome outcome = s3Client.GetObject(request);
  if (outcome.IsSuccess()) {
    oss << outcome.GetResult().GetBody().rdbuf();
    std::cout << oss.str() << std::endl;
  } else {
    std::cerr << "Failed to get the object data: " << outcome.GetError() << std::endl;
  }
}

int main() {
  Aws::SDKOptions options;
  Aws::InitAPI(options);
  int result = 0;

  {
    Aws::S3::S3ClientConfiguration config;

    /* std::string name = std::string(Aws::Region::AP_SOUTH_1); */
    /* std::cout << name << std::endl; */

    config.region = Aws::Region::EU_NORTH_1;

    Aws::S3::S3Client s3Client {config};
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

      /* storeData("Hello World again", s3Client); */
      /* deleteData(s3Client); */
      /* getData(s3Client); */
    }
  }
  Aws::ShutdownAPI(options);
  return result;
}

