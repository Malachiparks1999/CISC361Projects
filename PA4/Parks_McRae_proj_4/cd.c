/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 4
Due Date: 10/29/2020
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void cd(char *dir){
	if(dir != NULL){//  if not just cd
	char *cwd, *newcwd; // hold the cwd and new cwd
	cwd = getcwd(NULL,0);//setting up CWD
	int length = strlen(cwd)+strlen(dir)+2; // for null and /	
	int changedir = 0; // changes to 1 if dir changed
	//setting up newcwd
	newcwd = malloc(sizeof(char)*length);
	strcpy(newcwd,cwd);//cpy cwd into newcwd
	strcat(newcwd,"/");
	strcat(newcwd,dir);//append to directory
		if(access(newcwd,F_OK) == 0){//is a file
			if (ENOTDIR == errno){
				printf("%s is not a directory\n",dir);
				free(cwd);
				free(newcwd);
			}//if
			else{//file is directory and exists
				chdir(newcwd);//changes to new directory
				free(cwd);
				free(newcwd);
				changedir++;
			}//else		
		}//if
		if (strcmp(dir,"-") == 0){// move to previous dir
			chdir("..");
			free(cwd);
			free(newcwd);
			changedir++;
		}//if
		if(changedir == 0){
			printf("%s does not exist\n",dir);
			free(cwd);
			free(newcwd);
		}//else
	}//if
	else{//change to home
		char *home = getenv("HOME");
		chdir(home);
	}//else
}//cd
