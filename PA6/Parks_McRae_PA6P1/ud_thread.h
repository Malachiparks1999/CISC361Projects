/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 6
Due Date: 11/19/2020
*/

/* 
 * thread library function prototypes
 */
void t_create(void(*function)(int), int thread_id, int priority);
void t_yield(void);
void t_init(void);
void t_terminate(void);
void t_shutdown(void);
