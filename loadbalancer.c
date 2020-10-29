#include "commom.h"
#include "loadbalancer.h"
#include "file_process.h"
#define SERVER_INFO_FILE "server_info.txt"


int readServerInfoFile() {
  int i;
  FILE* fp = fopen(SERVER_INFO_FILE, "r");
  if (fp==NULL) {
    return -1;  // open info file error.
  }
  for (i=0; i<MAX_HTTP_SERVERS; i++) {
    char ip_addr[100];
    char port_in_char[11];
    int port;

    fgets_plus(ip_addr, 100, fp); // get ip.
    http_server_list0.server[i].server_addr.sin_addr.s_addr = inet_addr(ip_addr);
    strncpy(http_server_list0.server[i].ip_addr, ip_addr,19); // save in string as well.

    fgets_plus(port_in_char, 11, fp); // get ip.
    port = atoi(port_in_char);
    http_server_list0.server[i].server_addr.sin_port = htons(port);
    printf("machine : %s %d\n", http_server_list0.server[i].ip_addr, port);
  }
  fclose(fp);
  return 1;
}

int initHttpServerList() {
  http_server_list0.max_num = MAX_HTTP_SERVERS;
  http_server_list0.current_server_num = 0;
  int i;
  for (i=0; i< MAX_HTTP_SERVERS; i++) {
    http_server_list0.server[i].server_addr.sin_family = AF_INET;
    http_server_list0.server[i].server_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
    http_server_list0.server[i].server_addr.sin_port = htons(8888);
    http_server_list0.server[i].alive = 0;
  }
  int ret = readServerInfoFile();
  if (ret==-1) {
    return -1;
  }
  return 0;
}
//void takeLoadBalancerConnecion(int sockfd) {
//

int random_assign() {
  int a;
  a = (rand()%3);
  printf("random assign -> machine %d\n", a);
  return a;
}

int main() {
  srand(time(NULL));
  initHttpServerList();
  random_assign();  //test
  int sockfd, connectfd, len;
  struct sockaddr_in servaddr, client;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    exit(0);
  }
  
  memset(&servaddr, 0, sizeof(struct sockaddr_in) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(8081);
  
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

  // http request arrived
   int bytes;
   char buf[4096];
   char send_buf[4096];
   int rand_selected_server = random_assign();

   bytes = recv(connectfd, buf, 4096 ,0);
   
   // for website load balance (redirect)
   sprintf(send_buf, "HTTP/1.1 301 Moved Permanently\n");
   sprintf( send_buf + strlen(send_buf),  "Location: http://%s:8080/\n", http_server_list0.server[rand_selected_server].ip_addr);
   send(connectfd, send_buf, strlen(send_buf), 0);


  // load balance for hls downloader client   

    // code here...
  }
}

