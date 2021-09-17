/*
Part IV Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part IV
Due Date: 12/11/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include "ud_thread.h"

sem_t *s1, *s2;

void worker2(int thr_id) {
  printf("I am worker 2\n");
  sem_signal(s1);
  sem_signal(s1);
  t_terminate();
}

void worker1(int thr_id) {
  sem_wait(s1);
  printf("I am worker 1\n");
  sem_signal(s2);
  t_terminate();
}

void worker4(int thr_id) {
  sem_wait(s1);
  printf("I am worker 4\n");
  sem_signal(s2);
  t_terminate();
}

void worker3(int thr_id) {
  sem_wait(s2);
  sem_wait(s2);
  printf("I am worker 3\n");
  t_terminate();
}

int main(int argc, char *argv[])
{
  t_init();

  sem_init(&s1, 0);  
  sem_init(&s2, 0);  

  t_create(worker3, 3, 1);
  t_create(worker2, 2, 1);
  t_create(worker1, 1, 1); 
  t_create(worker4, 4, 1);
  t_yield();
  t_yield();
  t_yield();
  t_yield();
  printf("done.......\n");
  t_shutdown();
  return 0;
}
