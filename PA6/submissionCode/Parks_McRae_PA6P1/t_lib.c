/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6
Due Date: 11/19/2020
*/

#include "t_lib.h"

tcb *running;		//why make a queue when just one node
queue *ready;

void t_yield()
{
	// removing head of the ready queue
	tcb *prevReadyHead = NULL;
	if(ready->head != NULL){
		prevReadyHead = ready->head;
		ready->head = ready->head->next;
		prevReadyHead->next = NULL;
	}//if
	
	tcb *oldRunning = running;	//holds the old running
	oldRunning->next = NULL;	//make sure not connect
	//prevReadyHead->next = NULL;	//make sure isn't connect to anyting

	//adding to end of queue
	if(ready->head == NULL){
        	ready->head = oldRunning;
	} else{ //ready queue not empty
        	tcb *traverser; //used to traverse list
        	traverser = ready->head;
       		while(traverser->next != NULL){
                	traverser = traverser->next;
        	}//while
        	traverser->next = oldRunning;
	}//else

	//new running tcb
	running = prevReadyHead;

	// exhange stack information
	swapcontext(oldRunning->thread_context,prevReadyHead->thread_context);
}//t_yield

void t_init()
{

  // creating tcb block
  tcb *newtr;
  newtr = (tcb  *) malloc(sizeof(tcb));

  // setting up tcb block witi information on main
  newtr->thread_id = 0;
  newtr->thread_priority = 1;
  newtr->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t)); // creation of context of main
  newtr->next = NULL;

  getcontext(newtr->thread_context); // setting context to main()

  //setting tcb to running
  running = newtr;

  //setting up empty ready queue
  ready = (queue *) malloc(sizeof(queue));
  ready->head = NULL;
}//t_init



int t_create(void (*fct)(int), int id, int pri)
{

// when define STACKSIZE using #define in t_lib.h for some reason didn't work
size_t sz = 0x10000;

//Creating new tcb block
tcb *tr;
tr = (tcb *) malloc(sizeof(tcb));

//setting context for block
tr->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
getcontext(tr->thread_context);

// creating stack + initzalizing values
tr->thread_context->uc_stack.ss_sp = malloc(sz);
tr->thread_context->uc_stack.ss_size = sz;
makecontext(tr->thread_context, (void (*)(void)) fct,1,id);

// finishing setting up thread
tr->thread_id = id;
tr->thread_priority = pri;
tr->next = NULL;

//INSERT tr to the end of READY queue.

//if ready queue is empty
if(ready->head == NULL){
	ready->head = tr;
} else{ //ready queue not empty
	tcb *traverser;	//used to traverse list
	traverser = ready->head;
	while(traverser->next != NULL){
		traverser = traverser->next;
	}//while
	traverser->next = tr;
}//else

}//t_create

void t_terminate()
{
	free(running->thread_context->uc_stack.ss_sp);
	free(running->thread_context);
	free(running);

	// removing head of the ready queue
        tcb *prevReadyHead;
        if(ready->head != NULL){
                prevReadyHead = ready->head;
                ready->head = ready->head->next;
                prevReadyHead->next = NULL;
        }//if
	
	//restore running
	running = prevReadyHead;
	setcontext(running->thread_context);
}

void t_shutdown()
{
	printf("Clearing Memory\n");

	// used to clear out ready entire list
	tcb *traverser = ready->head;
	while(traverser->next != NULL){
		tcb *erase = traverser;
		traverser = traverser->next;
		free(erase->thread_context);
		free(erase);
	}//while
	
	//free last node in series of ready
	free(traverser->thread_context);
	free(traverser);

	// free running tcb
	free(running->thread_context);
	free(running);
}//t_shutdown()
