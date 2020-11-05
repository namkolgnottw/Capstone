#include "commom.h"
#include "check_status.h"
#include "network.h"
#define TOTAL_THREAD_NUM 5
#define WEB_SERVER1_IP "127.0.0.1"
#define WEB_SERVER2_IP "127.0.0.1"
#define WEB_SERVER3_IP "127.0.0.1"
#define BUFSIZE0 8192
#define M3U8_FILENAME "alita.m3u8"
#define MOVIE_NAME "alita"




int recv_file(int ser_fd, int file_size, char* buf, int buf_size) {
  memset(buf, 0, buf_size);
  int ret = 0;
  int len =file_size;
   printf("enter recv_file \n");
  int bytes;
  int recv_count = 0;
  while (1) {
    if (len==0) break;
    printf("testing 5\n");
    bytes = read(ser_fd, buf, buf_size);
    printf("testing 6\n");
    if (bytes>0) {
      len = len - bytes;
      recv_count  = recv_count + bytes;
    } else {
      break;
    }
    
    printf("testing 7\n");
  }
    printf("testing 8\n");
 
   printf("recv bytes = %d\n", recv_count);
  return recv_count;
}

int recv_hls_header(int ser_fd, char* buf, int buf_size, int* some_recv_body, int) {
  memset(buf, 0, buf_size);
 // printf("in recv_http_head : before read()\n");
  int bytes = read(ser_fd, buf, buf_size);
 // printf("in recv_http_head : after read()\n");
  if (strncmp(buf, "HLS:", 4)!=0)
    return -1; // recv wrong msg.

  char* ptr1 = strchr(buf, ':') + 1;
  char* ptr2 = strchr(ptr1, '\n');
  char content_len1[20];
  strncpy(content_len1, ptr1, ptr2 - ptr1);
  int content_len2 = atoi(content_len1);

  //char* resource_ptr = (char)malloc(content_len2);
  //memcpy()

  *some_recv_body = bytes - (ptr2 + 1 - buf);
  printf("some_recv_body = %d\n", *some_recv_body);

  return content_len2; 
}

//if redirect, return 2.
char* comfirm_recv(int ser_fd, char* buf, int buf_size) {
  //printf("enter comfirm_recv()\n");
  char* pos_ptr;  // may read more than the header.
 // printf("test2\n");
  int some_recv_body_bytes = 0;
  int ret0 = recv_hls_header(ser_fd, buf,buf_size, &some_recv_body_bytes, pos_ptr);  // get file length from header
  

  int file_len0 = ret0;

  if (some_recv_body_bytes == file_len0) {
    // small file will be recv in recv_hls_header  

  }

  //printf("some_recv_body_bytes = %d\n", some_recv_body_bytes);
  int file_len1 = ret0 - some_recv_body_bytes;
  int bytes_count = 0;
  printf("file length = %d\n", file_len0);
  int bytes = recv_file(ser_fd, file_len1, buf, buf_size);
  //printf("after recv body\n");
  bytes = bytes + some_recv_body_bytes;
  printf("bytes received=%d\n", bytes);

  close(ser_fd);

  if (bytes == file_len0)
    return 0;  // recv file compeletely
}

void send_m3u8_request(int ser_fd, char* movie_name, int user_id) {
  char msg[100];
  sprintf(msg, "HLS:M3U8;%d\n", user_id);
  sprintf(msg + strlen(msg), "%s.m3u8", movie_name);
  printf("m3u8 msg : %s", msg);
  write(ser_fd, msg, strlen(msg) );
}

void hls_download(int ser_fd, int user_index) {
  int ret;
  char msg_buf[128];
  char redirect_ip[20];
  int target_fd = ser_fd;
  int buf_size = BUFSIZE0;
  char buf[buf_size];
  char movie_name[30];
  strcpy(movie_name, MOVIE_NAME);
  
  send_m3u8_request(ser_fd, movie_name, user_index);

  ret = comfirm_recv(target_fd, buf, buf_size);
  if (ret == 0) 
    printf("user %d suceesful download alita.m3u8\n", user_index);

  
}


void* simulate_user1(void* index) {
  printf("testing3\n");
  int user_index  = *(int*)index;
  printf("user %d create.\n", *(int*)index);
  int target_ser_fd;
  int time = 12;
  target_ser_fd = connectServer(WEB_SERVER1_IP, HTTP_SERVER_PORT);
  if ( target_ser_fd < 0)
    return NULL;
  printf("user %d connected to the web server.\n", *(int*)index);

    printf("#%d start send HLS requset\n", user_index);
    hls_download(target_ser_fd, user_index);
    
}



int main() {
  pthread_t thread_id[TOTAL_THREAD_NUM];

  int i, ret;
  int index[TOTAL_THREAD_NUM];
  for (i=0; i<TOTAL_THREAD_NUM; i++) {
  printf("testing\n");
    index[i] = i;
  printf("testing2\n");
    pthread_create(&thread_id[i], NULL, simulate_user1, (void*)&index[i]);
  }
  


  for (i=0; i<TOTAL_THREAD_NUM; i++) {
    pthread_join(thread_id[i], NULL);
  }

    //pthread_join(thread_id[THREAD_NUM-1], NULL);
  
  // thread for check status
/*
  pthread_t thread_check_stat[3];
  for (i=0; i<1; i++) {
    pthread_create(&thread_check_stat[i], NULL, status_check, NULL);
  }
  for (i=0; i<1; i++) {
    pthread_join(thread_check_stat[i], NULL);
  }
  */
}
