#include "network.h"
#include "commom.h"
#include "file_process.h"



int sendFile(int client_fd, int file_size, char* resource_ptr) {
  int bytes;
  int sent_count = 0;
  int len = file_size;
  printf("in sendFile :  file size: %d\n", file_size);
  char buf[BUFSIZE];

  while(len!=0)  {

    if (len < BUFSIZE) { 
      memcpy(buf, resource_ptr+sent_count, len);
      write(client_fd, buf, len);
      sent_count = sent_count + len;
      len = len - len;
      
    } 

    else if (len >=BUFSIZE) {
      memcpy(buf, resource_ptr + sent_count, BUFSIZE);
      write(client_fd, buf, BUFSIZE);
      sent_count = sent_count + BUFSIZE;
      len = len - BUFSIZE;
     
    }
  //      printf("len=%d\n", len);
  //      printf("sent count= %d\n", sent_count);

  }
/*
  while (1) {
        if (sent_count >= file_size) break;
        bytes = write(client_fd, resource_ptr + sent_count, BUFSIZE);
        sent_count = sent_count + bytes;
        printf("bytes=%d\n", bytes);
        printf("sent count= %d\n", sent_count);
        if (bytes==-1 ){ 
          printf("send error.\n");
        break;
        }
  }
*/
  return len;
}

int readAndSend(const char* file_name, int client_fd) {
  int fd, bytes, n;
  int sent_count = 0;
  int len = getFileSize(file_name);
  char buf[BUFSIZE];
  if ( (fd = open(file_name, O_RDONLY)) ==-1 ) {  
    printf("open file error.\n");
    return -1;
  } 
                 while (1) {
                  if (len==0) break;

                  n = read(fd, buf, BUFSIZE);
                  if (n > 0) {
                  bytes = write(client_fd, buf, n) ;
                     sent_count = sent_count + bytes;
                   len = len - n;
                 }
               }

               printf("bytes sent = %d\n", sent_count);
  return sent_count;
}


// try 5 times to connect
int connect_plus() {
}



// try 5 times to bind
int bind_plus() {
}



// return a connect socket if succeed else return -1
int connectServer(const char* server_ip, int port) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd==-1)
    printf("error: open socket failed.\n");

  struct sockaddr_in cache_server1;
  memset(&cache_server1, 0, sizeof(struct sockaddr_in));
  cache_server1.sin_family = AF_INET;
  //cache_server1.sin_addr.s_addr = inet_addr(LOCALHOST);
  cache_server1.sin_addr.s_addr = inet_addr(server_ip);
  
  cache_server1.sin_port = htons(port);
  

  int ret = connect(sockfd, (struct sockaddr*)&cache_server1, sizeof(cache_server1) ); 

  if (ret == -1) {
    close(sockfd);
    return -1;
  }
  return sockfd;
}



int serverSideDeploy() {
}
