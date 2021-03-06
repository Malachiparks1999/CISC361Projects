/*
Part IV Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part IV
Due Date: 12/11/2020
*/

/* 
 * thread library function prototypes
 */
typedef void sem_t;  // for semaphore
typedef void mbox;   // for mailbox

void t_init(void);
void t_create(void(*function)(int), int thread_id, int priority);
void t_yield(void);
void t_terminate(void);
void t_shutdown(void);
int sem_init(sem_t **sp, unsigned int count);
void sem_wait(sem_t *sp);
void sem_signal(sem_t *sp);
void sem_destroy(sem_t **sp);
int mbox_create(mbox **mb); 
void mbox_deposit(mbox *mb, char *msg, int len);
void mbox_withdraw(mbox *mb, char *msg, int *len);
void receive(int *tid, char *msg, int *len);
void send(int tid, char *msg, int len);
void mbox_destroy(mbox **);
