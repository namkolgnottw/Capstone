#include <pthread.h>
#define THREAD_POOL_SIZE 200
struct thread {
int thread_type;  // 1 for socket

}

struct thread_list {
  pthread_t thread_id[THREAD_POOL_SIZE];
  int size;
}

struct thread_idle_queue {
  pthread_t thread_id[THREAD_POOL_SIZE];
  int size;
  int front;
  int rear;
}

void initThreadPool(int thread_num, int* current_pool_size) {

   struct thread_list thread_list0;  // Threads Creation.
   int i;
   for (i=0; i<THREAD_POOL_SIZE; i++) {
     pthread_create(&thread_list0[*current_pool_size], NULL, thread_init, NULL);
   } 
  

}
