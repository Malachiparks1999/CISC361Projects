/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/8/2020
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern char **environ;

int setEnv(char *envVarName, char *envVarValue){
	int retVal = 0;
	if(envVarName == NULL){//no arguments given
        	for(int i=0; environ[i] != NULL; i++){
			printf("%s\n",environ[i]);
                }//for
		return retVal;
        }//if
	if(envVarName != NULL && envVarValue == NULL){//empty string
		if(strcmp(envVarName,"PATH") == 0){//will not change path
			retVal = setenv(envVarName,"",0);
		}//if
		else{
			retVal = setenv(envVarName,"",1);
		}//else
		return retVal;
	}//if
	if(envVarName != NULL && envVarValue != NULL){//value
		 if(strcmp(envVarName,"PATH") == 0){
                        char *path, *newpath; // hold the cwd and new cwd
                        path = getenv(envVarName);//getting old path
                        int length = strlen(path)+strlen(envVarValue)+2; // for null and :

                        //setting up newpath
                        newpath = malloc(sizeof(char)*length);
                        strcpy(newpath,path);//cpy path into newpath
                        strcat(newpath,":");
                        strcat(newpath,envVarValue);//append to newpath
		
			retVal = setenv(envVarName,newpath,1);
			free(newpath);
                }//if
		else{
			retVal = setenv(envVarName,envVarValue,1);
		}//else
		return retVal;
	}//if
	return -1;
}//setenv

//use printenv in shell.c if arg is null
