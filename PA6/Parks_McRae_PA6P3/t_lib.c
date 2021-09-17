/*
Part III Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part III
Due Date: 12/3/2020
*/

#include "t_lib.h"


//	GLOBAL RUNNING TCBS
tcb *running;
tcb *ready;

//	MULTITHREAD LIBRARIES

void t_yield(){

  // pulling old running and store in tmp
  tcb* tmp;
  tmp = running; // store the currently running thread in tmp
  if(tmp != NULL){
  	tmp->next = NULL;
  }

  if (ready != NULL && running != NULL) { // only yield if there is a TCB in ready queue
    running = ready; // first TCB in ready queue becomes running
    ready = ready->next; // ready to next thread
    running->next = NULL;

    //iterate through ready queue to put old running at end of ready tcb
    tcb* iter;
    iter = ready;
    if (iter == NULL) // if there is nothing else in ready queue
      ready = tmp;
    else {
      while (iter->next != NULL) // loop till end of queue
        iter = iter->next;
      iter->next = tmp; //add tmp to end of queue
    }

    swapcontext(tmp->thread_context, running->thread_context);
  }
}//t_yield

void t_init(){

	// creating new tcb block
	tcb *newblock = (tcb *) malloc(sizeof(tcb));

	// setting up new tcb block with information
	newblock->thread_id = 0;
	newblock->thread_priority = 1;
	newblock->next = NULL;
	newblock->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));

	// setting context of new thread to main()
	getcontext(newblock->thread_context);
	
	// setting running to new tcb block
	running = newblock;

	// setting up empty ready queue
	ready = NULL;
}//t_init



int t_create(void (*fct)(int), int id, int pri){

	size_t sz = 0x10000;	//size of stacks for each thread

	//creation of tcb
	tcb *newtr;
	newtr = (tcb *) malloc(sizeof(tcb));

	// setting up context context of thread
	newtr->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
	getcontext(newtr->thread_context);

	// setting up stack portion of tcb
	newtr->thread_context->uc_stack.ss_sp = malloc(sz);
	newtr->thread_context->uc_stack.ss_size = sz;
	newtr->thread_context->uc_stack.ss_flags = 0;
	newtr->thread_context->uc_link = running->thread_context;

	// setting up rest of tcb
	newtr->thread_id = id;
	newtr->thread_priority = pri;
	newtr->next = NULL;

	// setting context to run functoin passed into t_create
	makecontext(newtr->thread_context, (void (*)(void)) fct,1,id);


	//INSERT tr to the end of READY queue.
	if(ready == NULL){	//empty ready queue
		ready = newtr;
	} else {	// non-empty queue
		tcb* traverser = ready;
		while(traverser->next != NULL){
			traverser = traverser->next;
		}// while
		traverser->next = newtr;
	}//else

	// Print statement for testing if reached end
	printf("New thread created\n");

}//t_create


// free entire TCB to avoid any memory buildup leaks
void t_shutdown(){
	if(ready != NULL){	// if ready queue isn't empty
		tcb *erase;	//used for erasing
		while(erase != NULL){
			erase = ready; //old head of ready
			ready = ready->next;	// make ready queue smaller
			
			//freeing memory in erase
			free(erase->thread_context->uc_stack.ss_sp);
			free(erase->thread_context);
			free(erase);
		}//while
	}//if
	
	//freeing running tcb
	free(running->thread_context);
	free(running);
	
	printf("All memory freed\n");
}//t_shutdown


/* Terminate the calling thread by removing (and freeing) its TCB from the
"running" queue, and resuming execution of the thread in the head of the
"ready" queue via setcontext(). */
void t_terminate(){
	tcb *erase;
	erase = running;
	running = ready;	//1st tcb block becomes new running

	if (ready != NULL){
		ready = ready->next;	// ready queue now points to next line
	}//if

	//freeing old running thread
	free(erase->thread_context->uc_stack.ss_sp);	// free stack
	free(erase->thread_context);	// free the old context
	free(erase);	//free tcb

	// resume running thread
	printf("Thread Terminated\n");
	setcontext(running->thread_context);
}//t_terminate

//	SEMAPHORES
int sem_init(sem_t **sp, int sem_count){	// sp is dbl ptr since &sp is used
  *sp = malloc(sizeof(sem_t));	// new space for semaphore
  (*sp)->count = sem_count;	// count of semaphore init
  (*sp)->blocked = NULL;	// queue of TCB ptr
}//sem_init


// forces a change of what queue is running if the count is < 0
void sem_wait(sem_t *s){
	s->count--;	//lowering count by 1 since p operation
	if(s->count < 0){	// if less then 0 keep adding "sleeping" old running threads 
		if(running != NULL){	//making sure running queue isn't empty

			if(s->blocked == NULL){	//if blocked queue is empty
  				s->blocked = running; // moving old running in to blocked queue
			} else { // if queue isn't empty append at end
				tcb *tmp = s->blocked;
				while(tmp->next != NULL){
					tmp = tmp->next;
				}//while
				tmp->next = running;
			} //else

			// moving ready queue to running
			running = ready;
			ready = ready->next;
			running->next = NULL;
		}//if
	}//if
}//sem_wait

void sem_signal(sem_t *s){
	s->count++;	// V operation so adding it back
	
	if(s->count <= 0){	//wake up older queues and add to end of ready
		if(running != NULL){	//running queue isn't empty and used on empty queues
			//removing old blocked head
			tcb * oldHead;
			if(s->blocked != NULL  && s->blocked->next == NULL){	//one blocked tcb left
				oldHead = s->blocked;
				s->blocked == NULL;
				oldHead->next = NULL;
			}//if
			else{	// multiple  length blocked
				oldHead = s->blocked;
				s->blocked = s->blocked->next;
				oldHead->next = NULL;
			}//else

			// adding tmp to end of ready queue
			tcb *tmp = ready;
			while(tmp->next != NULL){
				tmp = tmp->next;
			}// while
			tmp->next = oldHead;
		}//if
	}//if
}//sem_signal

void sem_destroy(sem_t **s){

	//moving entire blocked queue back into ready list
	tcb *iterator = (*s)->blocked;
	while(iterator != NULL){
		tcb *tmp = iterator;
		iterator = iterator->next;

		//adding tmp to end of ready
		tcb *readyItr = ready;
		while(readyItr->next != NULL){
			readyItr = readyItr->next;
		}//while
		readyItr->next = tmp;
	}//while

	// freeing semaphore memory
	free((*s)->blocked);
	free(*s);
}//sem_destroy


//	MESSAGE BOX

int mbox_create(mbox **mb){
  *mb = malloc(sizeof(mbox));
}//mbox_create

void mbox_deposit(mbox *mb, char *msg, int len){
}//mbox_deposit

void mbox_withdraw(mbox *mb, char *msg, int *len){
}//mbox_withdraw
