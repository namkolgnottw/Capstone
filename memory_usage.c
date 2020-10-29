#include "commom.h"
#include "memory_usage.h"

struct memory_usage {
  int mem_total;
  int mem_free;
};

void get_status(struct memory_usage* stat0) {
  FILE *fp = fopen("/proc/meminfo", "r");
  char str1[50];
  char str2[50];
  int mem_total, mem_free;
  fscanf(fp, "%s %d %s", str1, &mem_total, str2);
  fscanf(fp, "%s %d %s", str1, &mem_free, str2);
  stat0->mem_total = mem_total;
  stat0->mem_free = mem_free;
  fclose(fp);
}

int get_total_used_mem() {
  struct memory_usage stat0;
  get_status(&stat0);
  return stat0.mem_total - stat0.mem_free;
}

int get_total_mem() {
  struct memory_usage stat0;
  get_status(&stat0);
  return stat0.mem_total;
}

float get_percentage_total_used_mem() {
  struct memory_usage stat0;
  get_status(&stat0);
   float percentage = ( (float)(stat0.mem_total - stat0.mem_free) / (float)stat0.mem_total) *100;
}


void print_mem_usage() {
  float usage = get_percentage_total_used_mem();
  printf("Memory Used : %f %%\n", usage);
}

/*
int main() {

  struct memory_usage stat0;
  get_status(&stat0);
  printf("Memory total: %d\nMemory free: %d\n", stat0.mem_total, stat0.mem_free);
  print_mem_usage();
  return 0;
}

*/
