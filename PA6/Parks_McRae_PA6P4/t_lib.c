/*
Part IV Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part IV
Due Date: 12/11/2020
*/

#include <signal.h>
#include "t_lib.h"
#include <string.h>
#include <stdio.h>

tcb *running;
tcb *ready;

// SEMAPHORES

/* Create a new semaphore pointed to by sp with a count value of sem_count. */
int sem_init(sem_t **sp, int sem_count)
{
  sighold(SIGALRM);
  *sp = (sem_t*) malloc(sizeof(sem_t));
  (*sp)->count = sem_count;
  (*sp)->q = NULL;
  sigrelse(SIGALRM);
} 

/* Current thread does a wait (P) on the specified semaphore. */
void sem_wait(sem_t *sp)
{
  sighold(SIGALRM);
  sp->count--;
  if (sp->count < 0) { // to wait on semaphore
    tcb* tmp;
    tmp = running;
    tmp->next = NULL;
    running = ready; // move the 1st TCB in ready to running 
	
    if (ready != NULL)
      ready = ready->next;

    if (sp->q == NULL) 
      sp->q = tmp; // move running to sem's q
    else { 
      tcb* iter;
      iter = sp->q;
      while (iter->next != NULL) // loop to end of sem's q
        iter = iter->next; 
      iter->next = tmp; // add old running to end of sem's q
    }
    sigrelse(SIGALRM);

    swapcontext(tmp->thread_context, running->thread_context); 
  }
}

/* Current thread does a signal (V) on the specified semaphore. Follow the Mesa
 semantics where the thread that signals continues, and the first waiting (blocked)
 thread (if there is any) becomes ready. */

void sem_signal(sem_t *sp)
{
  sighold(SIGALRM);	
  sp->count++; //increase count

  if ((sp->q != NULL)) { // if q isn't empty, wake the 1st TCB and move it to the end of ready
    tcb* tmp;
    tmp = sp->q;
    sp->q = sp->q->next; // update head of sem's q
    if (ready == NULL)
      ready = tmp;
    else {
      tcb* iter;
      iter = ready;
      while (iter->next != NULL) // loop through ready queue
        iter = iter->next; // add awoken TCB to end of ready
      iter->next = tmp;
    }
    tmp->next = NULL; 	
  }
  sigrelse(SIGALRM);
}

/* Destroy (free) any state related to specified semaphore. */
void sem_destroy(sem_t **sp)
{
  sighold(SIGALRM);
  if ((*sp)->q != NULL) { // if sem's q isn't empty
    if (ready == NULL)
      ready = (*sp)->q; // move to ready 
    else {
      tcb* iter;
      iter = ready;
      while (iter->next!= NULL) // loop to end of ready q
	iter = iter-> next;
      iter->next = (*sp)->q; // add to end of ready q
    }
    (*sp)->q = NULL;
  }

  free(*sp); // free the semaphore
  sigrelse(SIGALRM);
}

void t_yield()
{
  sighold(SIGALRM);
  tcb* tmp;
  tmp =  running; //store the currently running htread in tmp
  tmp->next = NULL;
  
  if (ready != NULL) { //only yield if there is something in ready queue
	running = ready; //update running to first thread in ready queue
   	ready = ready->next; //update ready to next thread
  	running->next = NULL;
  	tcb* iter;
	iter = ready;
	if (iter == NULL) //if there is nothing else in ready queue
		ready = tmp;
	else { 
  		while (iter->next != NULL) //loop till end of queue
			iter = iter->next;
  		iter->next = tmp; //add tmp to end of queue
	}
	sigrelse(SIGALRM);
	swapcontext(tmp->thread_context, running->thread_context);
  }

}

// THREAD CREATION

 /* Initialize the thread library by setting up the "running" 
and the "ready" queues, creating TCB of the "main" thread, and inserting it into the running queue. */
void t_init()
{
	tcb *tmp = (tcb*) malloc(sizeof(tcb));
	tmp->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
	getcontext(tmp->thread_context);
	tmp->next = NULL;
	tmp->thread_id = 0;
	running = tmp;
	ready = NULL; 
}
/* Create a thread with priority pri, start function func with argument thr_id 
as the thread id. Function func should be of type void func(int). TCB of the newly
 created thread is added to the end of the "ready" queue; the parent thread calling
t_create() continues its execution upon returning from t_create(). */

int t_create(void (*fct)(int), int id, int pri)
{ 
  sighold(SIGALRM);
  size_t sz = 0x10000;
  tcb* tmp = (tcb*) malloc(sizeof(tcb));
  tmp->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));

  getcontext(tmp->thread_context);
/***
  uc->uc_stack.ss_sp = mmap(0, sz,
       PROT_READ | PROT_WRITE | PROT_EXEC,
       MAP_PRIVATE | MAP_ANON, -1, 0);
***/
  tmp->thread_context->uc_stack.ss_sp = malloc(sz);  /* new statement */
  tmp->thread_context->uc_stack.ss_size = sz;
  tmp->thread_context->uc_stack.ss_flags = 0;
  // tmp->thread_context->uc_link = running->thread_context; 
  makecontext(tmp->thread_context, (void (*)(void)) fct, 1, id);
  tmp->thread_id = id;
  tmp->thread_priority = pri;
  
  //adding mbox to creation
  mbox_create(&tmp->mailbox);

  tmp->next = NULL;

  if (ready == NULL)
	ready = tmp;
  else {
	tcb* t = ready;
	while(t->next!=NULL) {
		t = t->next;
	}
	t->next = tmp;
  }
  sigrelse(SIGALRM);
}

/* Terminate the calling thread by removing (and freeing) its TCB from the
"running" queue, and resuming execution of the thread in the head of the 
"ready" queue via setcontext(). */
void t_terminate()
{
	sighold(SIGALRM);
	tcb* tmp;
	tmp = running;
	running = ready;
	if (ready!=NULL)
		ready = ready->next;
	free(tmp->thread_context->uc_stack.ss_sp);
	free(tmp->thread_context);
	free(tmp);
	sigrelse(SIGALRM);
	setcontext(running->thread_context);
}

void t_shutdown()
{
	sighold(SIGALRM);
	if (ready!=NULL) {
		tcb* tmp;
		tmp = ready;
		while(tmp != NULL) {
			ready = ready->next;
			free(tmp->thread_context->uc_stack.ss_sp);
			free(tmp->thread_context);
			free(tmp);
			tmp = ready;
		}
	}
	free(running->thread_context);
	free(running);
	sigrelse(SIGALRM);	
}


// MESSAGE BOX

// Creates a new sized pointer to a new mailbox
int mbox_create(mbox **mb){
  mbox *newMailBox = malloc(sizeof(mbox));
  *mb = newMailBox;
  return 0;
}//mbox_create

// Destroy's a mailbox and frees the memory associated within it
void mbox_destroy(mbox **mailbox){
	free((*mailbox)->messageQueue);	//free the message held by the mailbox
	free((*mailbox)->mutex);	//free the lock associated with it
	free(mailbox);		//free the mailbox itself
}//mbox_destroy

// used for sending function where msg is placed into mailbox of mb
void mbox_deposit(mbox *mb, char *msg, int len){
	printf("Now Depositing Message\n");

	//store information in a message struct
	mBoxNode *newMessage = malloc(sizeof(msg));
	newMessage->message = msg;	//setting message pointer to the message
	newMessage->length = len;	// setting passed in length to message length

	// adding to mbox queue
	if(mb->messageQueue == NULL){	//queue is empty
		mb->messageQueue = newMessage;	// new message is front of queue
	}//if
	else{
		while(mb->messageQueue->next != NULL){	//iterate through list and add to the end
			mb->messageQueue = mb->messageQueue->next;
		}//while
		mb->messageQueue->next = newMessage;	//appended to end
	}//else
}//mbox_deposit	

// used from recieveing end to recieve the first node from Mbox
void mbox_withdraw(mbox *mb, char *msg, int *len){
	printf("Withdrawing first message\n");	//test to see if inside of function is even reached

	// if the mbox is empty just pass along null information
	char *passedMsg = "";
	*len = 0;

	// if not empty will move the information
	if(mb->messageQueue != NULL){
		*len = mb->messageQueue->length;	//length from message to new message
		passedMsg = mb->messageQueue->message; // saved passedMsg from mbox
		mb->messageQueue = mb->messageQueue->next; //advance queue since first is now removed
	}//if
	strcpy(msg,passedMsg);	//copy the withdrawn message to the new mbox
}//mbox_withdraw

// used to send message to other mailboxes in other threads
void send(int tid, char *msg, int len){
	mbox *destination = NULL; // in case there is no thread with the correct id

	// finding the correct thread
	if(running->thread_id == tid){  //running thread is the correct thread
        	destination = (mbox *) running->mailbox;
        }//if
        else{
                if(ready != NULL){      //ready queue isn't empty
                        tcb *temp = ready;      //used for tcb traversals
                        while(temp != NULL){    //tcb traversal
                                if(temp->thread_id == tid){     //if temps thread id matches
                                        destination = (mbox *) temp->mailbox;
                                }//if
                                temp = temp->next;
                        }//while
                }//if
        }//else

	// sending message to recipient
	mbox_deposit(destination, msg, len);
}//send

//used to recieve messages from sending threads
void receive(int *tid, char *msg, int *len){
	mbox *reciever = (mbox *) running->mailbox;	//running recieves the message
	if(tid == 0){	//if tid is 0 recieve from front of mailbox
		mbox_withdraw(reciever, msg, len);
	} // if
	else{	//tid is specified
        	printf("Withdrawing message from mbox with id %d\n", *tid); // test printing

        	// if the mbox is empty just pass along null information
        	char *passedMsg = "";
        	*len = 0;

        	// if not empty will move the information
        	if(reciever->messageQueue != NULL){
                while(reciever->messageQueue->next != NULL){ // list traversal to find tid mbox
                        if(reciever->messageQueue->sender == *tid){ //id's match
                                *len = reciever->messageQueue->length;        //length from message to ne>
                                passedMsg = reciever->messageQueue->message; // saved passedMsg from mbox
                        }//if
                                reciever->messageQueue = reciever->messageQueue->next; //advance queue since fi>
                }//while
        }//if
        strcpy(msg,passedMsg);  //copy the withdrawn message to the new mbox
	}//else
}//recieve
