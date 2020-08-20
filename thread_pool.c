#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread_pool.h"
#include "http_server.h"


void print_idle_queue() {
  int i;
  int queue_current_thread = thread_idle_queue0.thread_num;
  //int front = thread_idle_queue0.front;
  //int rear = thread_idle_queue0.rear;
  //printf("current front = %d\n", front);
  printf("thread idle queue :\n");
  for (i=0; i<202; i++) {
    if (thread_idle_queue0.queue[i]!=NULL) 
      printf("index-%d: %d ", i, thread_idle_queue0.queue[i]->index);
  }
}


void thread_enqueue(struct thread_unit* thread0) {
  int rear = thread_idle_queue0.rear;

  if (rear == (thread_idle_queue0.max_size-1) )
    thread_idle_queue0.rear = 0;
  else
    thread_idle_queue0.rear++;

  rear = thread_idle_queue0.rear;
  thread_idle_queue0.queue[rear] = thread0;
}

// can not dequeue before the first enqueue.
struct thread_unit* thread_dequeue() {
  int front = thread_idle_queue0.front;
  struct thread_unit* thread_ptr = thread_idle_queue0.queue[front];

  if (front != (thread_idle_queue0.max_size-1) )
    thread_idle_queue0.front++;
  else
    thread_idle_queue0.front = 0;

  return thread_ptr;
}

void initThreadPool() {
  //static struct thread_pool thread_pool0;
  int init_thread_num = 40;
  thread_pool0.created_thread_num = init_thread_num;;
  thread_pool0.maximum_thread_num = 200; // alter later
  thread_pool0.mode = 1;
  int tmp_max = thread_pool0.maximum_thread_num;
  pthread_t thread_id[tmp_max];
  // 1.update thread pool. 
  // create thread_unit, assign to pointer of thread pool
  int i;
  for (i=0; i<tmp_max; i++) {

    thread_pool0.thread[i] = (struct thread_unit *)malloc(sizeof(struct thread_unit)); 
    thread_pool0.thread[i]->index = i;
    thread_pool0.thread[i]->type = 1; // alter later
    thread_pool0.thread[i]->sock_fd = -1;
    thread_pool0.thread[i]->state = -1;
  }
  
  // 2.init idle queue.
   // static struct thread_pool_idle_queue thread_idle_queue0;
    thread_idle_queue0.max_size = tmp_max + 2;
    thread_idle_queue0.thread_num = 0;
    thread_idle_queue0.front = 0;
    thread_idle_queue0.rear = -1;    


  // 3.create/activate a partial threads e.g. 40/200
  // linking crated thread to struct thread_unit
   // update thread state and tid.
   int tmp_index[init_thread_num];
  for (i=0; i<init_thread_num; i++) {
     tmp_index[i] = i;
     pthread_create(&thread_id[i], NULL ,thread_running, (void*)&tmp_index[i]);
    thread_pool0.thread[i]->tid = thread_id[i];
    thread_pool0.thread[i]->state = 0;  // init thread will be all idle.
    thread_enqueue(thread_pool0.thread[i]);
    
  }



}
void take_thread_tohandle(int sockfd) {
  // 1.select a thread in the front of queue :              threadq_dequeue()
  struct thread_unit* thread_ptr = thread_dequeue();
  // 2. update struct thread_pool's struct thread_unit status
  // 2.5 (copy)update thread_unit sockfd to connect to exact sockfd and thread.
  thread_ptr->type = 1;
  thread_ptr->sock_fd = sockfd;
  // 3. awake the thread(int func: thread_running)
  pthread_cond_signal(&thread_ptr->cond_var);
  
  

}

struct thread_unit* getThread(pthread_t tid) {
  int i;
  for (i=0; i<THREAD_POOL_SIZE; i++) {
    if ( thread_pool0.thread[i]->tid == tid )
      return thread_pool0.thread[i];
  }
  printf("can not find the thread from thread pool by tid.\n ");
  return NULL;
}

// main program of threads.
void* thread_running(void* thread_index) {
  thread_pool0.created_thread_num++;
  pthread_t tmp_id = pthread_self();
  int index = *(int*)thread_index;
  int thread_state = thread_pool0.thread[index]->state;
  struct thread_unit* thread0 = thread_pool0.thread[index];
  thread_state = 0; //init to idle.
  for (;;) {
    switch (thread_state) {
      case 0: //idle
        printf("thread %d status : waiting\n\n", index);
        pthread_cond_wait(&thread0->cond_var, &thread0->mutex);
        printf("thread %d status : be awaken\n\n", index);
        thread_state = 1;
        break; 
  
      case 1:  //working ; 
         switch (thread0->type) {
           case 1: takeHttpConnection(thread0->sock_fd); break;
          // case 2: OtherRquest();
          // case 2: CheckCacheServer();
         }
           thread_state = 0;
           thread_enqueue(thread0);
        printf("thread %d status : waiting\n\n", index);
         break;
      
      case -1: break; // state -1 for close the thread
    }
  } 
  
}

  


