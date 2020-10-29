
struct ServerStat {
  int thread_num;
  int mem_used;
  int mem_total;
  float mem_usage;  //percentage
  float loadavg_1min;
  float loadavg_5min;
  float loadavg_15min;
  int transfer_bytes_1min;
};

void printServerStat(struct ServerStat* ser_stat0);
void status_check(int mins);
