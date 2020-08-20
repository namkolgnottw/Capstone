#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <ctype.h>
#include "file_process.c"

  // wait for tcp connection
  // multi-thread
  // wait for asking for session information
  // wait for the image request before going to database server.

  // manage the ranking list made by the logging.
  // auto reading the file when the list updated.  
void FreeAllMalloc() {
}

void LoadFileByRankList() {
  // tmp data for rank list
  rank_list0.list_size = 1;
  rank_list0.ranking_table[0].type = 3;
  char* file_name1 = "html/loginpage.html"; //tmp
  strcpy(rank_list0.ranking_table[0].file_name, file_name2);
  
  int list_size = rank_list0.list_size;
  int i;
  for (i=0; i<list_size1; i++) {
    char file_name[100];
    strcpy(file_name, rank_list0.ranking_table[i].file_name);
    rank_list0.ranking_table[i].resource_ptr = readFileToMemory(file_name);
  }

}


// if not found, return NULL; 
char* FindFileFromRankList(const char* file_name) {
  int list_size = rank_list0.list_size;
  int i;
  char* resource_ptr = NULL;
  int i;
  for (i=0; i<list_size1; i++) {
    if ( strcmp(rank_list0.ranking_table[i].file_name, file_name) == 0 ) {
      resource_ptr = ranklist0.ranking_table[i].resource_ptr;
      break;
    }
  }
  return resource_ptr;
}

void initCacheServer() {
  //initRankList();
  LoadFileByRankList();
}

void* connect_handler(void* sockfd) {
  char read_buf[4096];
  int sock_fd = *(int*)sockfd;
  read(sock_fd, read_buf, 4096);
  printf("%s", read_buf);
// take the first message, for regonize the operation
  //e.g. IS-MSG:CS1 for check the cache for a http request, (IS for inter server, MSG for message)(CS for cache server)
  //e.g. inter-server-massge:CS2  for transfer a set of log record.
  if (strncmp(read_buf, "IS-MSG:CS1")==0)
    opcode = 1;
  else if (strncmp(read_buf, "IS-MSG:CS2")==0)
    opcode = 2;

  memset(read_buf, 0, sizeof(read_buf) );
  //read the second massge
  read(sock_fd, read_buf, 4096);
  printf("%s\n", read_buf);
  if ( strncmp(read_buf, "IS-MSG:CS",9)==0 ) {
    opcode = read_buf[9] - '0';
  } else { // not IS-MSG
    printf("not IS-MSG");
    return 0;
  }
 
  int opcode;
  switch(opcode) {
    case 1:  // get file name from buffer
            char* resource_ptr = FindFileFromRankList(file_name);
            if (resource_ptr!=NULL) {
             send(fd, resource_ptr, strlen(resource_ptr), 0) ;
            } else {
             send(fd, no_file_msg, strlen(no_file_msg), 0) ;
            }
              //response IS-MS:WS1 means no suct file
            
           break;
    case 2:
           break;
  }
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
  serv_addr.sin_port = htons(10000);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));
  if (listen(listenfd, 4)==-1) {
    printf("listen failed.");
    exit(0);
  }
  int len;

  pthread_t thread_id;
  while (socketfd = accept(listenfd, (struct sockaddr*)&client_addr, &len)) {
    printf("Connection accepted.\n");
    if ( pthread_create(&thread_id, NULL, connection_handler, (void*)&socketfd) < 0 )
      printf("Thread creation error.\n");
  }
  return 0;
}
