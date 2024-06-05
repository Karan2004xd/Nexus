#ifndef CONSTANTS_H_IN
#define CONSTANTS_H_IN

#define DEFAULT_PORT 8082
#define DEFAULT_HOST "localhost"

#define FILE_NAME "filename"
#define FILE_CONTENT "content"
#define CACHE_DIR "/home/karan/Documents/Code/Nexus/main/.NexusCache"

#define MAIN_DIR "/home/karan/Documents/Code/Nexus/main"
#define EXT ".txt"

// Queries Constants
#define QUERIES_DIR "/home/karan/Documents/Code/Nexus/main/metadata/queries"
#define TEST_QUERIES_DIR "/home/karan/Documents/Code/Nexus/main/metadata/queries/test"

#define DATA_QUERIES_DIR "/home/karan/Documents/Code/Nexus/main/metadata/queries/data"
#define CACHE_QUERIES_DIR "/home/karan/Documents/Code/Nexus/main/metadata/queries/cache"

#define STORAGE_QUERIES_DIR "/home/karan/Documents/Code/Nexus/main/metadata/queries/storage"
#define DFS_QUERIES_DIR "/home/karan/Documents/Code/Nexus/main/metadata/queries/dfs"

#define USER_QUERIES_DIR "/home/karan/Documents/Code/Nexus/main/metadata/queries/user"

// Interface 
#define PATH_TO_INTERFACES ""
#define PATH_TO_ASSETS ""
#define PATH_TO_STYLES ""

#define OPERATION "operation"
#define CLIENT_USERNAME "username"
#define CLIENT_PASSWORD "password"

// Request operations
#define PUT_DATA "put-data"

#define GET_DATA "get-data"
#define GET_TRASH_DATA "get-trash-data"

#define DELETE_DATA "delete-data"
#define DELETE_TRASH_DATA "delete-trash-data"

#define LIST_DATA "list-data"
#define LIST_TRASH_DATA "list-trash-data"

#define ADD_USER "add-user"
#define DELETE_USER "delete-user"
#define CHECK_USER "check-user"

// Data-Transfer
#define KAFKA_BROKER ""
#define KAFKA_NEXUS_BACKEND_TOPIC ""
#define KAFKA_NEXUS_BACKEND_GROUP_ID ""
#define KAFKA_NEXUS_FRONTEND_GROUP_ID ""
#define KAFKA_NEXUS_FRONTEND_TOPIC ""

#endif // CONSTANTS_H_IN
