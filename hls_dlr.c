#define PORT 8080
#include "commom.h"
#include <pthread.h>

struct player {
  pthread_t thread_id;
  int current_time;
  int whole_movie_time;
};

int request_m3u8(int sockfd, struct sockaddr_in* server_addr0, char* file_name, char* file_ptr, int file_size) {
  
}

int request_ts(int index0, int sockfd, struct sockaddr_in* server_addr0, char* file_name, char* file_ptr, int* file_size0) {

  char read_buf[BUFSIZE];
  int file_size;
  int len0;
  int connect_fd = connect(sockfd, (struct sockaddr*)&server_addr0, sizeof(server_addr0) );

  if ( connect_fd) {
  printf("thread %d connected the server successfully.\n\n", index0 );
  }

  else {
    printf("thread %d connection failed.\n\n", index0);
    return -1;
  }

  // send first hls msg get the file

  printf("ready to send first msg\n");
  char hls_msg1[256];
  sprintf(hls_msg1, "HLS:TS\n%s", file_name);
  write(sockfd, hls_msg1, strlen(hls_msg1) );
 
  // received a respone to get the file size.
  read(sockfd, read_buf, BUFSIZE);
  char* pos_ptr = strchr(read_buf, '\n') + 1; 
  if ( atoi(pos_ptr) )
    file_size = atoi(pos_ptr);
  else 
    return -1;
  printf("%s file size = %d\n", file_name, file_size);
  // send second hls msg to tell server is ok to send
  char hls_msg2[16];
  sprintf(hls_msg2, "HLS OK");    
  write(sockfd, hls_msg2, strlen(hls_msg2) );
  
  

  // receive the file.
  file_ptr = (char*)malloc(file_size);
  memset(read_buf, 0, BUFSIZE);
  int len = file_size;
  int bytes;
  int recv_count = 0;

  while (1) {

    if (len==0) break;
    bytes = read(sockfd, read_buf, BUFSIZE);

    if (bytes > 0) {
      memcpy(file_ptr + recv_count, read_buf, bytes);
      len = len - bytes;
      recv_count = recv_count + bytes;
    }
    else {
      break;
    }
  }

  if (file_size != recv_count) {
    free(file_ptr);
    file_ptr = NULL;
    return -1;
  }

  *file_size0 = file_size;
  return file_size;

}

int request_movie_catalog() {
}

void* thread_connect(void* index) {

    printf("testing\n");
  int sockfd, connect_fd;
  struct sockaddr_in server_addr, client_addr;
  int index0 = *(int*)index;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == - 1) {
    printf("connection failed.\n");
    exit(0);
  }

  printf("thread %d connecting the server ...\n\n", index0);
  
  memset(&server_addr, 0, sizeof(server_addr) );

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
  server_addr.sin_port = htons(HTTP_SERVER_PORT);


  
  char* movie_name = "alita";
  char file_name1[1024]; 
  char *file_ptr;
  int file_size1;
  strncpy(file_name1, movie_name, strlen(movie_name) );
  strncpy(file_name1 + strlen(file_name1), ".m3u8", strlen(".m3u8") );
  int ret1 = request_ts(index0, sockfd, &server_addr, file_name1, file_ptr, &file_size1);
  if (ret1==-1) {
    printf("request file failed.\n");
    exit(0);
  }

 // calculate the movie time.

 // after received the first file, start the clock

}


int main() {

  printf("welcome to the hls downloader, this program simulate thethousands of user playing the movie through hls protocol.\n\n");

  printf("please enter the total number of  simulate users\n");

  int user_num = 10;
  pthread_t thread_id_list[user_num];
  // creating thread for connection 
  int i;
  int index[user_num];
  for (i=0; i<user_num; i++) {
    index[i] = i;
    pthread_create(&thread_id_list[i], NULL, thread_connect, (void*)&index[i]);
  } 
  while(1) {
    sleep(1);
  }
}
