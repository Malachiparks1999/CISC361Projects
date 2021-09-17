/*
Editors: Malachi Parks
Section: CISC361-010
Assignment: Homework Assignment: Semaphores
Due Date: 11/17/2020
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s; 

void *child(void *arg) {
    printf("child\n");
    sem_post(&s);	//add +1 back to sempaphore
    sleep(1);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    sem_init(&s,0,0);
    printf("parent: begin\n");
    pthread_create(&p, NULL, child, NULL);
    sem_wait(&s);   //decrement to wait
    printf("parent: end\n");
    return 0;
}
