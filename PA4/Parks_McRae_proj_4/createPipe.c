/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 4
Due Date: 10/29/2020
*/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void createPipe(char ** inputArgs){
	int pid;
	int pipefd[2];

	pipe(pipefd);
	pid = fork();

        if(pid == 0){            //call fork here, if  not child won't execute this part, supposed to write
       		close(1);
                dup(pipefd[1]); // pipe's have own stdout, setting pipe stdout to stdout
		close(pipefd[0]); // close since using unilateral pipe
                execlp(inputArgs[0],inputArgs[0],NULL);
        }
       	else{ // parent executes this, supposed to read
        	close(0);       // closing stdin since parent is reading in child stdout
                dup(pipefd[0]); // setting up stdin of pipe to stdin
		close(pipefd[1]); // close to prevent non stop issue
                execlp(inputArgs[2], inputArgs[2], inputArgs[3], NULL);
        }
}


void createPipeErr(char ** inputArgs){
        int pid;
        int pipefd[2];

        pipe(pipefd);
        pid = fork();

        if(pid == 0){            //call fork here, if  not child won't execute this part, supposed to write
                close(1);
		close(2);
                dup(pipefd[1]); // pipe's have own stdout, setting pipe stdout to stdout
		dup(pipefd[2]); // pipping stderr to parent stdin also
                close(pipefd[0]); // close since using unilateral pipe
                execlp(inputArgs[0],inputArgs[0],NULL);
        }
        else{ // parent executes this, supposed to read
                close(0);       // closing stdin since parent is reading in child stdout
                dup(pipefd[0]); // setting up stdin of pipe to stdin
                close(pipefd[1]); // close to prevent non stop issue
                execlp(inputArgs[2], inputArgs[2], inputArgs[3], NULL);
        }
}
