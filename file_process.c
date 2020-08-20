#include <socket.h>




// the file in memory
int send_file(int sock, struct sockaddr_in* addr0) {

}

// establish file in current directory
int create_file(char* file_path, char* buf) {

}


// file size need to be known before used this func.
int getFileToBuf(char* buf, const char* file_name) {
  int fd;
  fd = open(file_name, O_RDONLY);
  while (read(fd, buf + strlen(buf), 4096) > 0 )
    continue;
  close(fd);
}

char* readFileToMemory(char* file_name) {
  int file_size;
  struct stat st;
  stat(file_name, &st);
  file_size = st.st_size;
  char* resource_ptr = (char*)malloc(file_size);
  getFileToBuf(resource_ptr);

}
