/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/8/2020
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigIntHandler(int sigNum){
	char *cwd = getcwd(NULL,0);
	printf("\n[%s]$ ",cwd);
	fflush(stdout);
	free(cwd);
}//sigInt Handler
