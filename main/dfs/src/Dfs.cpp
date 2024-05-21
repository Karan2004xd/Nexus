#include "../include/Dfs.hpp"
#include "../../data/include/Contents.hpp"
#include "../../data/include/Chunker.hpp"
#include "../../storage/include/AwsS3.hpp"

using namespace Nexus;
  /* std::string output; */
  /* DfsResult::ResultType resultType; */
  /* try { */
  /* } catch (std::exception &e) { */
  /*   output = e.what(); */
  /*   resultType = DfsResult::ResultType::FAILED; */
  /* } */
  /* return {output, resultType}; */

DfsResult Dfs::storeDataApi(const std::string &fileName,
                            const std::string &fileContent) {
  std::string output, errorMsg;
  DfsResult::ResultType resultType;
  try {
    Data::Contents contents {fileName, fileContent};
    Data::Chunker chunker {contents};
    auto &chunks = chunker.getChunks();

    Storage::AwsS3 aws;
    aws.storeData(chunks);
  } catch (std::exception &e) {
    errorMsg = e.what();
    output = "Failed to create new file";
    resultType = DfsResult::ResultType::FAILED;
  }
  return {output, errorMsg, resultType};
}
