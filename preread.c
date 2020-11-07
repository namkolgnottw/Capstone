#define PRE_READ_BUFFER_SIZE 50

struct buf {
  int ts_file_index;
  char* resource_ptr;
};

struct buf pre_read_buf[PRE_READ_BUFFER_SIZE];


char* getTsFromBuf(int ts_file_index0) {
  char* ts_file_ptr = NULL;

  int i;
  for (i=0; i<PRE_READ_BUFFER_SIZE; i++) {
    if (pre_read_buf[i].ts_file_index == ts_file_index0)
    return pre_read_buf[i].resource_ptr;
  }

  return ts_file_ptr;
}
