#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "master_http_server.h"

void initHttpServerList() {
  http_server_list0.max_num = MAX_HTTP_SERVERS;
  http_server_list0.current_server_num = 0;
  int i;
  for (i=0; i< MAXT_HTTP_SERVERS; i++) {
    http_server_list0.http_server[i].server.sin_family = AF_INET;
    http_server_list0.http_server[i].server.s_addr = in ;
    http_server_list0.http_server[i].server.sin_port = htons(8888);
    http_server_list0.http_server[i].server.alive = 0;
  }
}

void addNewServer(struct socksddr_in server) {
    int index = http_server_list0.current_server_num;
    if (index == max_num) {
      printf("New Server adding failed : Server num has reached the maximum.\n");
      return;
    }
    http_server_list0.http_server[i].server.s_addr = server.s_addr ;
    http_server_list0.http_server[i].server.alive = 1;
  
}

int main() {
  int sockfd, connectfd, len;
  struct sockaddr_in servaddr, client;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
}
