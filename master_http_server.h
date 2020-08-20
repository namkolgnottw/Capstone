#include <pthread.h>

#define MAX_HTTP_SERVERS 20


struct http_server {
  struct socksddr_in server;
  int alive;
  int connected_socket;
  int loading; //1-10.
  
};

struct http_server_list {
  struct http_server[MAX_HTTP_SERVERS];
  int current_server_num;
  int max_num;
};

struct http_server_list0;
