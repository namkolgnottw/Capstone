#define CACHE_SERVER_PORT 10000

struct cache_server{
  int id;
  int status; // 0 for shutdown, 1 for alive
  struct sockaddr_in addr;
};

struct cache_server_list{
  struct cache_server[10]; // default for 10
  int current_num;
  int total;
};

struct resource{
  int type; // 1 for picture, 2 for video, 3 for..
  int file_size;
  char file_name[100]; // include name
  char* resource_ptr; // the file that read into memory.
};


// default for 10 top hit access resources
struct ranking_list{
  struct resource ranking_table[10];
  int list_size;
};


// write into the logging file everytime the server recived a set of 50 log records.
// or take the recent 500 logging file to statistic and ranking the resource ranking list.
struct log{
  int time;
  int date;
  char msg[512];
};

//// global variables ////
struct cache_server_list cache_server_list0;

// for cache_server.c
struct ranking_list rank_list0;
