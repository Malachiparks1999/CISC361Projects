/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/8/2020
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern char **environ; // used to access enviroment

void printenv(char *envVar){
	if(envVar == NULL){//no arguments given
		for(int i=0; environ[i] != NULL; i++){
			printf("%s\n",environ[i]);
		}//for
	}//if
	else{// argument given
		char *temp;// hold value of getenv
		temp = getenv(envVar);
		if(temp == NULL){
			printf("%s does not exist\n",envVar);
		}//if
		else{
			printf("%s\n",getenv(envVar));
		}//else
	}//else
}//printenv

