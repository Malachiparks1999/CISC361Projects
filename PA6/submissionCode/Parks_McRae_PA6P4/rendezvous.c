/*
Part IV Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part IV
Due Date: 12/11/2020
*/

/* 
 * Semaphore - rendezvous
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ud_thread.h"

sem_t *s1, *s2;

void child_1(int arg) 
{
  printf("child 1: before\n");
  // sleep(4);
  sem_signal(s2);
  sem_wait(s1);
  printf("child 1: after\n");

  t_terminate();
}

void child_2(int arg) 
{
  printf("child 2: before\n");
  sem_signal(s1);
  // sleep(3);
  sem_wait(s2);
  printf("child 2: after\n");

  t_terminate();
}

int main(void) 
{
  int i;

  t_init();
  sem_init(&s1, 0);
  sem_init(&s2, 0);

  t_create(child_1, 1, 1);
  t_create(child_2, 2, 1);
  
  t_yield();
  t_yield();

  sem_destroy(&s1);
  sem_destroy(&s2);
  t_shutdown();

  return 0;
}
