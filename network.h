#define CACHE_SERVER_PORT 8181
#define HTTP_SERVER_PORT 8080

int connectServer(const char* server_ip, int port);
int readAndSend(const char* file_name, int client_fd);
int sendFile(int client_fd, int file_size, char* resource_ptr);
