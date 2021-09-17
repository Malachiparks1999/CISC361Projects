#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "common_threads.h"

sem_t s1, s2;

void *worker1(void *arg) {
  printf("worker 1 is scheduled\n");
  sem_wait(&s2);
  printf("I am worker 1\n");
  pthread_exit(NULL);
}

void *worker2(void *arg) {
  printf("worker 2 is scheduled\n");
  sem_wait(&s1);
  printf("I am worker 2\n");
  sem_post(&s2);
  pthread_exit(NULL);
}

void *worker3(void *arg) {
  printf("worker 3 is scheduled\n");
  printf("I am worker 3\n");
  sem_post(&s1);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t p1, p2, p3;

  sem_init(&s1, 0, 0);  
  sem_init(&s2, 0, 0);  

  Pthread_create(&p1, NULL, worker1, NULL); 
  Pthread_create(&p2, NULL, worker2, NULL);
  Pthread_create(&p3, NULL, worker3, NULL);
  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  Pthread_join(p3, NULL);
  printf("done.......\n");
  return 0;
}
