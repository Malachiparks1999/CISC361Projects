/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/8/2020
*/

#include "get_path.h"

void where(char *command, struct pathelement *p){
  char cmd[128];
  if(command == NULL){
	printf("command not found\n");
  }
  else{
  	while (p) {       
    		sprintf(cmd, "%s/%s", p->element, command);
    			if (access(cmd, X_OK) == 0) {
      				printf("%s\n",cmd);
    			}//if
    		p = p->next;
  	}//while
  }//else
}//where
