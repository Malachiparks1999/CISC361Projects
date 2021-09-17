/*
Part III Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part III
Due Date: 12/3/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include "ud_thread.h"

// Order should be 2 1 4 3

sem_t *s1, *s2, *s3;

void worker1(int id) {
  printf("worker 1 is scheduled\n");
  sem_wait(s1);	//used to lock where worker 2 will unlock this block
  printf("I am worker 1\n");
  sem_signal(s2);	//used to unlock worker 3
  t_terminate();
}

void worker2(int id) {
  printf("worker 2 is scheduled\n");
  printf("I am worker 2\n");
  sem_signal(s1);	// used to unlock worker 1
  t_terminate();
}

void worker3(int id) {
  printf("worker 3 is scheduled\n");
  sem_wait(s3);	//used to lock where worker 4 will remove this block
  printf("I am worker 3\n");
  t_terminate();
}

void worker4(int id) {
  printf("worker 4 is scheduled\n");
  sem_wait(s2);	// used to lock where worker 1 will remove the block
  printf("I am worker 4\n");
  sem_signal(s3);	// used to unlock worker 3
  t_terminate();
}

int main(int argc, char *argv[])
{
  t_init();

  //init set to 0 so when waiting doesn't return automatically
  sem_init(&s1,0);
  sem_init(&s2,0);
  sem_init(&s3,0);

  t_create(worker3,1,1);
  t_create(worker4,2,1);
  t_create(worker2,3,1);
  t_create(worker1,4,1);
  t_yield(); 
  //t_shutdown();
  printf("done.......\n");
  return 0;
}
