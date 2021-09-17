/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/8/2020
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *prompt(char *enteredPrompt){
	//used to create new path
        char *cwd, *newPrompt; // hold the cwd and new cwd
        cwd = getcwd(NULL,0);//setting up CWD
        int length = strlen(cwd)+strlen(enteredPrompt)+4; // for null and " "

        //setting up newcwd
        newPrompt = malloc(sizeof(char)*length);
        strcpy(newPrompt,enteredPrompt);//cpy cwd into newcwd
        strcat(newPrompt," [");
        strcat(newPrompt,cwd);//append to directory Name
	strcat(newPrompt,"]");
	free(cwd);
	return newPrompt;
}//prompt
