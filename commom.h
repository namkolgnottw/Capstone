#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>

#define BUFSIZE 8192
#define LOCAL_HOST "127.0.0.1"
#define HTTP_SERVER_PORT 8080
