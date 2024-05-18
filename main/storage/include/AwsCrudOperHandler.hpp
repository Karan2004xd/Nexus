#ifndef AWS_CRUD_OPER_HANDLER_HPP
#define AWS_CRUD_OPER_HANDLER_HPP

#include <aws/s3/S3ClientConfiguration.h>

class AwsCrudOperHandler {
protected:
  void putData(const Aws::S3::S3ClientConfiguration &config,
               const std::string &objectKey,
               const std::string &bucketName,
               const std::string &data);

  void deleteData(const Aws::S3::S3ClientConfiguration &config,
                  const std::string &bucketName,
                  const std::string &objectKey);

  std::string getData(const Aws::S3::S3ClientConfiguration &config,
                      const std::string &bucketName,
                      const std::string &objectKey);
};

#endif // AWS_CRUD_OPER_HANDLER_HPP
