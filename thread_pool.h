#define THREAD_POOL_SIZE 200

struct thread_unit {
int index;
int type;  // 1 for http socket
int sock_fd;
int cache_server_sock_fd;
int linked_cache_server_id;
int state; // 0 for idle, 1 for busy, -1 for dead
pthread_t tid;
pthread_cond_t cond_var;
pthread_mutex_t mutex;

};

struct thread_pool {
  struct thread_unit *thread[THREAD_POOL_SIZE];
  int created_thread_num;
  int maximum_thread_num;
  int mode;  // 1 for default, 2 for medium, 3 for buzy, 4 for full-power
};

struct thread_pool_idle_queue {
  struct thread_unit* queue[THREAD_POOL_SIZE+2];
  int max_size;
  int thread_num;
  int front;
  int rear;
};

////global varialbes////
struct thread_pool thread_pool0;
struct thread_pool_idle_queue thread_idle_queue0;
///////////////////////


////functions//////////
void thread_enqueue(struct thread_unit* thread0);
struct thread_unit* thread_dequeue();
void take_thread_tohandle(int sockfd);
void* thread_running();
void initThreadPool();
void print_idle_queue();


