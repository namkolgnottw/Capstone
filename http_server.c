#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <ctype.h>
#include "thread_pool.h"
#include "cache_server.h
"
#define STATCODE_404 "404 Bad Request\n"
#define STATCODE_200 "200 OK\n"
#define STATCODE_204 "No Content\n"
#define MAX_THREAD_NUM 20
#define BUFSIZE 8096
#define BLOCK_SIZE 4096

// Global variables for init.
// init the whole program structure

void initStructure() {
  initThreadPool();
 // initHttpServer();
  printf("testing : initThreadPool completed\n");
  
}

/////////////////////////////////
/// Cache server info need to add into cache server info txt file manually.
////////////////////////////////

void ReadCacheSerInfo() {
  int fd;
  if ( (fd = open( "/info/cache_server_info.txt", O_RDONLY))==-1) {
  }
  while (read(fd, write_buf + strlen(write_buf), 1024)>0) continue;

}

int AskCacheSer_File(int cache_server_sockfd) {
 // get the a idle cache server from cache_server_list0, the cache_server_list.cache_server[0] will be the lowest loading one.
  int sockfd, connfd;
  char write_buf[4096], read_buf[4096];
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd==-1) {
    // report error
    printf("error: open socket failed.\n");
  }
  memset(cache_server_list.cache_server[0].addr, 0, sizeof(struct sockaddr_in) );
  struct cache_server cache_server1 = cache_server_list.cache_server[0];
  // temp code:
  struct sockaddr_in addr1 = cache_server1.addr;
  addr1.sin_family = AF_INET;
  addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr1.sin_port = htons(CACHE_SERVER_PORT);
  
  if ( cache_server1.status == 1) {
    connect(sockfd, (struct sockaddr*)&addr1, sizeof(addr1) );  
    sprintf(write_buf, "IS-MSG:CS1");
    write(sockfd, write_buf, sizeof(write_buf));
    read(sockfd, read_buf, sizeof(read_buf));
    printf("read_buf:%s\n", read_buf);
    if (strncmp(read_buf, "IS-MSG:RESOURCE EXISTED", 23)==0) {
      return 1;  // 1 represent existed.

    } else {
      printf("resource not found from cache server.\n"); 
      return 0; 
    }
    
  }
    //report error : no cache server alive.
    return 0;
}

int GetFileFromCacheSer(char* buf_ptr, int* file_size) {

}




void getMethod(const char* http_msg_buf, int client_fd) {

  char write_buf[BUFSIZE], buffer[BUFSIZE];
  char file_name[100];
  // cut file name after the '/'
  char* ptr = strchr(http_msg_buf, '/')+1;
  if (*ptr == ' ') {
    sprintf(file_name, "html/index.html");
  } else {
    char* second_blank_pos;
    second_blank_pos = strchr(http_msg_buf, ' ') + 1;
    second_blank_pos = strchr(second_blank_pos, ' ');
    printf("\n%s", second_blank_pos);
    printf("\n%s", ptr);
    int file_name_len = second_blank_pos - ptr;
  sprintf(file_name, "html/");
  strncpy(file_name + 5, ptr , file_name_len);
  printf("filename:%s\n",file_name);
  printf("file_name_length:%d\n", file_name_len);
  }


  sprintf(write_buf, "HTTP/1.1 ");
  sprintf(write_buf + strlen(write_buf), STATCODE_200);
  sprintf(write_buf+strlen(write_buf), "Access-Control-Allow-Origin: *\n");
  sprintf(write_buf + strlen(write_buf), "Content-Type: text/html; charset=utf-8\n");
  sprintf(write_buf + strlen(write_buf), "Server: MyWebServer\n\n");

  int file_size;
  int boolean0 = AskCacheSer_File(); 
  if ( boolean0 == 1) { //file exsited in cache ser
    GetFileFromCacheSer(write_buf + strlen(write_buf), &file_size);
  }

  int fd;
  if ( (fd = open( file_name, O_RDONLY))==-1) {
           memset(write_buf, 0, sizeof(write_buf)); 
           sprintf(write_buf, STATCODE_204);               
         } else {
           printf("Resource exist.\n");
           while (read(fd, write_buf + strlen(write_buf), 1024)>0) continue;
        }

      send(client_fd, write_buf, strlen(write_buf), 0);
      close(client_fd);

}




// how to get the file descriptor here?
void takeHttpConnection(int socket_fd) {

  char read_buf[4096], write_buf[BUFSIZE], buffer[BUFSIZE];
  int socketfd = socket_fd; 
  int fd, file_fd, ret;
  FILE* fp;
  
  read(socketfd, read_buf, 4096);
  printf("%s", read_buf);
      

    if(strncmp(read_buf, "GET", 3)==0 ) {    
      getMethod(read_buf, socket_fd);
      return;
    }

    // Bad request.
    memset(write_buf, 0, sizeof(read_buf));
    printf("404 Bad Request.\n");
    printf("\n\n%s", read_buf);
    sprintf(write_buf , "HTTP/1.1 ");
    sprintf(write_buf + strlen(write_buf), STATCODE_404);
    //sprintf(write_buf + strlen(write_buf), "Connection: close\n");
    write(socketfd, write_buf, strlen(write_buf));
   
  return;      
}

int main() {
  initStructure();
  printf("testing : structure init completed \n");
  // accecpt tcp connection
  int sockfd, connectfd, len;
  struct sockaddr_in servaddr, client;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    //socket creation failed
    exit(0);
  }
  printf("socket created.\n");
  
  memset(&servaddr, 0, sizeof(struct sockaddr_in) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(8080);
  
  if ( bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr) ) ) {
    printf("bind failed.\n");
    exit(0);
  }
  if ( listen(sockfd, 50) != 0 ) {
    printf("listen failed.\n");
  }
  len = sizeof(client);
  while (1) {
    connectfd = accept(sockfd, (struct sockaddr*)&client, &len );
    // test here.
    printf("a connection accepted.\n");;
    take_thread_tohandle(connectfd);
  // http request arrived
  }
  
  
  return 0;
}

