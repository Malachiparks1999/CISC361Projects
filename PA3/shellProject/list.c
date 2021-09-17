/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/8/2020
*/

#include <stdlib.h>
#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <unistd.h>

int list(char *dirName) {
  DIR *d;
  struct dirent *dir;
  
  if(dirName == NULL){//print current working directory with no args
  	d = opendir(".");
  	if (d) {
    		while ((dir = readdir(d)) != NULL) {
      			printf("%s\n", dir->d_name);
    		}//while
    	closedir(d);
  	}//if
  }//if
  else{
	//used to get complete working path of dir to try and open it
        char *cwd, *newcwd; // hold the cwd and new cwd
        cwd = getcwd(NULL,0);//setting up CWD
        int length = strlen(cwd)+strlen(dirName)+2; // for null and /
        
        //setting up newcwd
        newcwd = malloc(sizeof(char)*length);
        strcpy(newcwd,cwd);//cpy cwd into newcwd
        strcat(newcwd,"/");
        strcat(newcwd,dirName);//append to directory Name

	d = opendir(newcwd);
	if (d) {
		while((dir = readdir(d)) != NULL){
			printf("%s: %s\n",dirName,dir->d_name);
		}//while
	closedir(d);
	}//if
	free(cwd);
	free(newcwd);
  }//else
}//list

// in shell just have ls loop if more than one argument using list
// have dirname: filename for printing
