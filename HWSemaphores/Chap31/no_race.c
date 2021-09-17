#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "common_threads.h"

sem_t s;

volatile int counter = 0; 
int loops;

void *worker(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    sem_wait(&s);
    counter = counter + 1;
    sem_post(&s);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if (argc != 2) { 
    fprintf(stderr, "usage: threads <loops>\n"); 
    exit(1); 
  } 
  loops = atoi(argv[1]);
  pthread_t p1, p2;
  sem_init(&s, 0, 1);  
  printf("Initial value : %d\n", counter);
  Pthread_create(&p1, NULL, worker, NULL); 
  Pthread_create(&p2, NULL, worker, NULL);
  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  printf("Final value   : %d\n", counter);
  return 0;
}
