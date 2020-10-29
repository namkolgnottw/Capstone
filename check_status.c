#include "commom.h"
#include "network.h"
#include "check_status.h"

#define WEB_SERVER1_IP "127.0.0.1"
#define WEB_SERVER2_IP "127.0.0.1"
#define WEB_SERVER3_IP "127.0.0.1"


int cal_transfer_times(int mins) {
  // check status for every 5 second.
  int times = mins * 12;
  return times;
}

void toServerStat(struct ServerStat* ser_stat0, char* server_msg) {
  
  sscanf(server_msg, "%d %d %d %f %f %f %f %d" , &ser_stat0->thread_num, &ser_stat0->mem_used, &ser_stat0->mem_total, &ser_stat0->mem_usage, &ser_stat0->loadavg_1min, &ser_stat0->loadavg_5min, &ser_stat0->loadavg_15min, &ser_stat0->transfer_bytes_1min);

}

void printServerStat(struct ServerStat* ser_stat0) {
  printf("%d %d %d %f %f %f %f %d\n" , ser_stat0->thread_num, ser_stat0->mem_used, ser_stat0->mem_total, ser_stat0->mem_usage, ser_stat0->loadavg_1min, ser_stat0->loadavg_5min, ser_stat0->loadavg_15min, ser_stat0->transfer_bytes_1min);

}

void status_check(int mins) {
  int target_ser_fd;
  target_ser_fd = connectServer( WEB_SERVER1_IP, HTTP_SERVER_PORT);

  char msg[128];
  sprintf(msg, "STAT CHECK:%d", mins);
  write(target_ser_fd, msg, strlen(msg));
  
  int num_check = cal_transfer_times(mins);
  struct ServerStat ser_stat0[num_check];
  char buf[128];
  int i;
  
  for (i=0; i<num_check; i++) {
    read(target_ser_fd, buf, 128);
    toServerStat(&ser_stat0[i], buf);
    printServerStat(&ser_stat0[i]);
  }

  close(target_ser_fd);
  return;
}


/*
int main() {

  status_check(1);
}
*/
