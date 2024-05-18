#include "../include/AwsCrudOperHandler.hpp"

#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>

void AwsCrudOperHandler::putData(const Aws::S3::S3ClientConfiguration &config,
                                 const std::string &bucketName,
                                 const std::string &objectKey,
                                 const std::string &data) {
  Aws::S3::S3Client s3Client {config};

  Aws::S3::Model::PutObjectRequest request;
  request.SetBucket(bucketName);
  request.SetKey(objectKey);

  auto dataStream = Aws::MakeShared<Aws::StringStream>("Data");
  *dataStream << data;
  request.SetBody(dataStream);

  auto putObjectOutcome = s3Client.PutObject(request);
  if (putObjectOutcome.IsSuccess()) {
    std::cout << "Object uploaded successfully" << std::endl;
  } else {
    std::cerr << "Failed to upload object to S3: " <<  putObjectOutcome.GetError() << std::endl;
    throw std::runtime_error("AwsCrudOperHandler Error (putData)");
  }
}

void AwsCrudOperHandler::deleteData(const Aws::S3::S3ClientConfiguration &config,
                                    const std::string &bucketName,
                                    const std::string &objectKey) {
  Aws::S3::S3Client s3Client {config};

  Aws::S3::Model::DeleteObjectRequest request;
  request.WithKey(objectKey).WithBucket(bucketName);

  Aws::S3::Model::DeleteObjectOutcome outcome = s3Client.DeleteObject(request);

  if (outcome.IsSuccess()) {
    std::cout << "Successfully deleted object" << std::endl;
  } else {
    std::cerr << "Failed to delete the object: " << outcome.GetError() << std::endl;
    throw std::runtime_error("AwsHandler Error (deleteData)");
  }
}

std::string AwsCrudOperHandler::getData(const Aws::S3::S3ClientConfiguration &config,
                                        const std::string &bucketName,
                                        const std::string &objectKey) {
  Aws::S3::S3Client s3Client {config};
  std::ostringstream oss;
  Aws::S3::Model::GetObjectRequest request;
  request.SetBucket(bucketName);
  request.SetKey(objectKey);

  Aws::S3::Model::GetObjectOutcome outcome = s3Client.GetObject(request);

  if (outcome.IsSuccess()) {
    oss << outcome.GetResult().GetBody().rdbuf();
  } else {
    std::cerr << "Failed to get the object data: " << outcome.GetError() << std::endl;
    throw std::runtime_error("AwsHandler Error (getData)");
  }
  return oss.str();
}
