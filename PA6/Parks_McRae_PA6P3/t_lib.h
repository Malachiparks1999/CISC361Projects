/*
Part III Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part III
Due Date: 12/3/2020
*/

/*
 * types used by thread library
 */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>

struct tcb {
  int         thread_id;
  int         thread_priority;
  ucontext_t *thread_context;
  struct tcb *next;
};

typedef struct tcb tcb;

typedef struct {
  int count;
  tcb *blocked;
} sem_t;

typedef struct msg {
  char       *message;
  int         length;
  struct msg *next;
} msg;

typedef struct {
  struct mboxnode *first;
  struct mboxnode *last;
  sem_t *mutex;
} mbox;

// Similar to push function
void insertAtEnd(tcb *,tcb *);

// Similar to pop function
tcb *removeFront(tcb *);
