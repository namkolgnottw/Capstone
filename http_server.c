#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <ctype.h>

#define STATCODE_404 "404 Bad Request\n"
#define STATCODE_200 "200 OK\n"
#define STATCODE_204 "No Content\n"
#define MAX_THREAD_NUM 20
#define BUFSIZE 8096
#define BLOCK_SIZE 4096

// Global variables for init.



/////////////////////////////////



int close_file = 0;

void* connection_handler(void* socket_fd) {

  char read_buf[4096], write_buf[BUFSIZE], buffer[BUFSIZE];
  int socketfd = *(int*)socket_fd; 
  int fd, file_fd, ret;
  long len;
  FILE* fp;
  
  read(socketfd, read_buf, 4096);
  printf("%s", read_buf);
      

    if(strncmp(read_buf, "GET", 3)==0 ) {    
        sprintf(write_buf, "HTTP/1.1 ");
        sprintf(write_buf + strlen(write_buf), STATCODE_200);
        sprintf(write_buf+strlen(write_buf), "Access-Control-Allow-Origin: *\n");
        sprintf(write_buf + strlen(write_buf), "Content-Type: text/html; charset=utf-8\n");
        sprintf(write_buf + strlen(write_buf), "Server: MyWebServer\n\n");
        //printf("file ready to open.\n");
        printf("test.\n");
        if ( (fd = open( "index.html", O_RDONLY))==-1) {
           memset(write_buf, 0, sizeof(write_buf)); 
           sprintf(write_buf, STATCODE_204);               
         } else {
           printf("Resource exist.\n");
           while (read(fd, write_buf + strlen(write_buf), 1024)>0) continue;
        }
      //printf("%s",write_buf);
      send(socketfd, write_buf, strlen(write_buf), 0);
      memset(read_buf, 0, sizeof(read_buf));
      memset(write_buf, 0, sizeof(write_buf));
      close(socketfd);
      return 0;
    }
    // Bad request.
    memset(write_buf, 0, sizeof(read_buf));
    printf("404 Bad Request.\n");
    printf("\n\n%s", read_buf);
    sprintf(write_buf , "HTTP/1.1 ");
    sprintf(write_buf + strlen(write_buf), STATCODE_404);
    //sprintf(write_buf + strlen(write_buf), "Connection: close\n");
    write(socketfd, write_buf, strlen(write_buf));
   
  return 0;      
}

int main() {

  int listenfd, socketfd, pid;
  int fd;
  static struct sockaddr_in serv_addr;
  static struct sockaddr_in client_addr;
  char *seg_ptr;
  FILE *fp;
  
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  //inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));
  if (listen(listenfd, 4)==-1) {
    printf("listen failed.");
    exit(0);
  }
  int len;

    printf("listening.\n");
    //socketfd = accept(listenfd, (struct sockaddr*)&client_addr, &len);
    //printf("A socket connected.\n");

  pthread_t thread_id;
  while (socketfd = accept(listenfd, (struct sockaddr*)&client_addr, &len)) {
    printf("Connection accepted.\n");
    if ( pthread_create(&thread_id, NULL, connection_handler, (void*)&socketfd) < 0 )
      printf("Thread creation error.\n");
    //if (close_file==1) {
    //  close(socketfd);
    //  return 0;
    //}
  }
  return 0;
}

