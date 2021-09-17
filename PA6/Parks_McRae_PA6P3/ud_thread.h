/*
Part III Header:
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6 - Part III
Due Date: 12/3/2020
*/

/* 
 * thread library function prototypes
 */
typedef void sem_t;  // semaphore
typedef void mbox;   // mailbox

void t_create(void(*function)(int), int thread_id, int priority);
void t_yield(void);
void t_init(void);
void t_terminate(void);
void t_shutdown(void);
int sem_init(sem_t **sp, unsigned int count);
void sem_wait(sem_t *sp);
void sem_signal(sem_t *sp);
void sem_destroy(sem_t **sp);
int mbox_create(mbox **mb); 
void mbox_deposit(mbox *mb, char *msg, int len);
void mbox_withdraw(mbox *mb, char *msg, int *len);
