/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6
Due Date: 11/19/2020
*/

/*
 * types used by thread library
 */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>

// struct of a tcb which is held in a SLL
struct tcb {
  int         thread_id;
  int         thread_priority;
  ucontext_t *thread_context;
  struct tcb *next;
};

typedef struct tcb tcb;

// struct of queues to create running and ready queue
struct queue
{
    struct tcb  *head;
};

typedef struct queue queue;
