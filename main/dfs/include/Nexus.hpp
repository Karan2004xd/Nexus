#ifndef NEXUX_HPP
#define NEXUX_HPP

namespace Nexus {
  class Dfs;
  class DfsResult;
  class Server;

  namespace Utils {
    class SimpleJsonParser;
    class SimpleQueryParser;
    class RequestParser;
  }
  
  class MetaData;

  namespace Data {
    class Cache;
    class Contents;
    class Chunker;
  }

  namespace Storage {
    class AwsS3;
  }

  class User;
  class DataTransfer;
}
#endif // NEXUX_HPP
