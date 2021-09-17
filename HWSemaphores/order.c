/*
Editors: Malachi Parks
Section: CISC361-010
Assignment: Homework Assignment: Semaphores
Due Date: 11/17/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

// Order should be 2 1 4 3

sem_t s1, s2, s3;

void *worker1(void *arg) {
  printf("worker 1 is scheduled\n");
  sem_wait(&s1);	//used to lock where worker 2 will unlock this block
  printf("I am worker 1\n");
  sem_post(&s2);	//used to unlock worker 3
  pthread_exit(NULL);
}

void *worker2(void *arg) {
  printf("worker 2 is scheduled\n");
  printf("I am worker 2\n");
  sem_post(&s1);	// used to unlock worker 1
  pthread_exit(NULL);
}

void *worker3(void *arg) {
  printf("worker 3 is scheduled\n");
  sem_wait(&s3);	//used to lock where worker 4 will remove this block
  printf("I am worker 3\n");
  pthread_exit(NULL);
}

void *worker4(void *arg) {
  printf("worker 4 is scheduled\n");
  sem_wait(&s2);	// used to lock where worker 1 will remove the block
  printf("I am worker 4\n");
  sem_post(&s3);	// used to unlock worker 3
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t p1, p2, p3, p4;

  //init set to 0 so when waiting doesn't return automatically
  sem_init(&s1,0,0);
  sem_init(&s2,0,0);
  sem_init(&s3,0,0);

  pthread_create(&p3, NULL, worker3, NULL);
  pthread_create(&p4, NULL, worker4, NULL);
  pthread_create(&p2, NULL, worker2, NULL);
  pthread_create(&p1, NULL, worker1, NULL); 
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  pthread_join(p3, NULL);
  pthread_join(p4, NULL);
  printf("done.......\n");
  return 0;
}
