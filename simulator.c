#include "commom.h"
#include "check_status.h"
#include "network.h"
#define THREAD_NUM 200
#define WEB_SERVER1_IP "127.0.0.1"
#define WEB_SERVER2_IP "127.0.0.1"
#define WEB_SERVER3_IP "127.0.0.1"
#define HOME_PAGE 0
#define BUFSIZE0 8192

struct page {  // the page may have pics
  int total_resource;  //total
  char filename[100];
  char resource_filename[50][100];
};

/////// global variable /////////
struct page page_list[5];

/////////////////////////////////

void initStructPage() {

  page_list[0];  // home page
  sprintf(page_list[0].filename, "/index.html");
  page_list[0].total_resource = 1;
  sprintf(page_list[0].resource_filename[0], "/home_page1.jpg" );

  page_list[1];  // catalog page
  sprintf(page_list[1].filename, "/movie_catalog.html");
  page_list[1].total_resource = 4;
  sprintf(page_list[1].resource_filename[0], "/alita.jpg" );
  sprintf(page_list[1].resource_filename[1], "/avengers-endgame.jpg" );
  sprintf(page_list[1].resource_filename[2], "/dunkirk.jpg" );
  sprintf(page_list[1].resource_filename[2], "/star-wars-ep7.jpeg" );
}

void toHttpRequestMsg(char* msg_buf, char* file_path) {
   sprintf(msg_buf, "GET %s HTTP/1.0", file_path);
}

int recv_http_header(int ser_fd, char* buf,int buf_size, char* ptr0, int* some_recv_body) {
  
  memset(buf, 0, buf_size);
  printf("in recv_http_head : before read()");
  int bytes = read(ser_fd, buf, buf_size);
  printf("in recv_http_head : after read()");
  if (strncmp(buf, "HTTP/1.1 301 Moved Permanently", strlen("HTTP/1.1 301 Moved Permanently") ) == 0) {
    return -2;
  }
  if (strncmp(buf, "HTTP/1.1 200 OK", 15)!=0)
    return -1; // recv wrong msg.

  char* ptr1 = strstr(buf, "Content-Length:") + strlen("Content-Length:");
  char* ptr2 = strchr(ptr1, '\n');
  char content_len1[20];
  strncpy(content_len1, ptr1, ptr2 - ptr1);
  int content_len2 = atoi(content_len1);

  while ( (ptr0 = strstr(buf, "\n\n") ) == NULL) {
    memset(buf, 0, buf_size);
    bytes = bytes + read(ser_fd, buf, buf_size);
  }
  ptr0 = ptr0 + 2;
  //*some_recv_body = strlen(ptr0);
  *some_recv_body = bytes - (ptr0 - buf);

  return content_len2; 
}

int recv_http_body(int ser_fd, int file_size, char* buf, int buf_size) {
  //char buf[512];
  memset(buf, 0, buf_size);
  int ret = 0;
  int len =file_size;
  // printf("enter recv_http_body() \n");
  int bytes;
  int recv_count = 0;
  while (1) {
    if (len==0) break;
    bytes = read(ser_fd, buf, buf_size);
    if (bytes>0) {
      len = len - bytes;
      recv_count  = recv_count + bytes;
    } else {
      break;
    }
    
  }
 
  // printf("recv bytes = %d\n", recv_count);
  return recv_count;
}

//if redirect, return 2.
int comfirm_recv(int ser_fd, char* redirect_ip, int* redirect_host, char* buf, int buf_size) {
  // check redirect first;
  //printf("enter comfirm_recv()\n");
  //char buf1[256];
  char* pos_ptr;  // may read more than the header.
   //printf("waiting http header from server.\n");
 // printf("test2\n");
  int some_recv_body_bytes = 0;
  //printf("before_entering recv_http_header\n");
  int ret0 = recv_http_header(ser_fd, buf,buf_size, pos_ptr, &some_recv_body_bytes);  // get file length from header

  if (ret0 == -1) return -1;  // wrong msg
  if (ret0 == -2) return 2;
  int file_len0 = ret0;
  //printf("some_recv_body_bytes = %d\n", some_recv_body_bytes);
  int file_len1 = ret0 - some_recv_body_bytes;
  int bytes_count = 0;
  //bytes_count = bytes + strlen(pos_ptr);
  printf("file length = %d\n", file_len0);
   // printf("waiting http BODY from server\n");
  //printf("before recv body\n");
  int bytes = recv_http_body(ser_fd, file_len1, buf, buf_size);
  //printf("after recv body\n");
  bytes = bytes + some_recv_body_bytes;
  printf("bytes received=%d\n", bytes);
  if (bytes >= file_len0  && bytes < (file_len0+10) )
    return 0;  // recv file compeletely
}

void http_visit(int ser_fd, int page0, int user_index) {
  int ret;
  char msg_buf[128];
  char redirect_ip[20];
  int redirect_port;
  int target_fd = ser_fd;
  int buf_size = BUFSIZE0;
  char buf[buf_size];
  // file path?
  //printf("user %d ready to send http request\n", user_index);
  
  // get the page it self 
  toHttpRequestMsg(msg_buf, page_list[page0].filename);
 // printf("user %d at stage create httpmsg\n", user_index);
  
  write(target_fd, msg_buf, strlen(msg_buf)); // send request
  //printf("user %d sent request\n", user_index);
  //printf("testing.\n");
  
  ret = comfirm_recv(target_fd, redirect_ip, &redirect_port, buf, buf_size);
  if (ret == 0) 
    printf("user %d suceesful access page:%s ", user_index, page_list[page0].filename);

  
  if ( ret == 2) {  // redirect
    printf("user %d redirecting\n", user_index);
    close(ser_fd);
    target_fd = connectServer(redirect_ip, redirect_port);
    
  }
  //printf("ready to access resource\n"); 
  int i, ret1;
  int total_resource = page_list[page0].total_resource;
  printf("total resource = %d\n", total_resource);
  for (i=0; i < total_resource; i++) {
    //printf("try to connection");
    target_fd = connectServer(WEB_SERVER1_IP, 8080);

    //printf("connect established.\n");

    toHttpRequestMsg(msg_buf, page_list[page0].resource_filename[i]);
    write(target_fd, msg_buf, strlen(msg_buf));
    //printf("sent request resource msg \n");
    ret1 = comfirm_recv(target_fd, redirect_ip, &redirect_port, buf, buf_size);

    if (ret1 == 0) 
      printf(" user %d suceesful access page:%s ", user_index, page_list[page0].resource_filename[i]);
    
    close(target_fd);
  }
}


// visit home page
void* simulate_user1(void* index) {
  int user_index  = *(int*)index;
  printf("user %d create.\n", *(int*)index);
  int target_ser_fd;
  int time = 1;
  int i;
  for (i=0; i<time; i++) {
  target_ser_fd = connectServer(WEB_SERVER1_IP, HTTP_SERVER_PORT);
  if ( target_ser_fd < 0)
    return NULL;
  printf("user %d connected to the web server.\n", *(int*)index);

    printf("#%d requset\n", i);
    http_visit(target_ser_fd, HOME_PAGE, user_index);
    
    //sleep(1);
  }
}

int main() {
  initStructPage();
  pthread_t thread_id[THREAD_NUM];

  int i, ret, index;
 
  for (i=0; i<THREAD_NUM; i++) {
    index = i;
    pthread_create(&thread_id[i], NULL, simulate_user1, (void*)&i);
    printf("i = %d\n", i);
  }
  for (i=0; i<THREAD_NUM; i++) {
    pthread_join(thread_id[THREAD_NUM-1], NULL);
  }
    //pthread_join(thread_id[THREAD_NUM-1], NULL);
  /*
  // thread for check status
  for (i=0; i<3; i++) {
    pthread_t pthread_create(&thread_id[i], NULL, check_status(1), (void*)index);
  }
  */
  
}
