/*
Part IV Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part IV
Due Date: 12/11/2020
*/

/*
 * types used by thread library
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>

struct tcb {
  int         thread_id;
  int         thread_priority;
  ucontext_t  *thread_context;
  struct tcb *next;
  struct mbox *mailbox;	//used to hold and pass messages
};

typedef struct tcb tcb;

typedef struct {
  int count;
  tcb *q;
} sem_t;

typedef struct mBoxNode {
  char       *message;	// message being passed
  int         length;	// length of the message
  int 	      sender;   // who created the message
  struct mBoxNode *next;	// next message within the queue
} mBoxNode;

typedef struct {
  struct mBoxNode *messageQueue;
  sem_t *mutex;
} mbox;
