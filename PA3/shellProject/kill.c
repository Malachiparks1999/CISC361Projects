/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/8/2020
*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int killProc(int passedPID, char *killtype){
	int exist = kill(passedPID,0);
	if(exist == -1){//does not  exist
		return -1;
	}//if
	if(killtype == NULL){//killed in passed command
		kill(passedPID,15);
		return 0;
	}//
	else{//selected kill type
		int sig = atoi(killtype);
		int offset = -(sig*2);
		sig = sig + offset;
		kill(passedPID,sig);
		return 0;
	}//else
}//kill

//add definition to sh.h
