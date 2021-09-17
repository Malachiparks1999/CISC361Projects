/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/8/2020
*/

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"

int main(int argc, char **argv, char **envp) {
	char	buf[MAXLINE];
	char    *arg[MAXARGS];  // an array of tokens
	char    *ptr;
        char    *pch;
	pid_t	pid;
	int	status, i, arg_no;
	char	*newPrompt;		//prompt of shell
	char	*cwd; 			//current working dir
	int	promptNotRun = 0;		// if prompt not run print cwd flag
	int	builtInRun = 0;			//if built in cmd ran flag is 1, prevents other print
	
	//signal handlers
	signal(SIGINT, sigIntHandler);//ignore ctrl c, does not kill children processes yet
	signal(SIGTSTP,sigIntHandler);//ignores ctrl z

	// print prompt of cwd then freeing it
	newPrompt = getcwd(NULL,0);
	printf("[%s]$ ",newPrompt);	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (strlen(buf) == 1 && buf[strlen(buf) - 1] == '\n'){//hit enter key
			goto reprompt;
		}//if
		
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0; /* replace newline with null */

		// parse command line into tokens
		arg_no = 0;
                pch = strtok(buf, " ");
                while (pch != NULL && arg_no < MAXARGS)
                {
		  arg[arg_no] = pch;
		  arg_no++;
                  pch = strtok (NULL, " ");
                }
		arg[arg_no] = (char *) NULL;
		// print tokens
		for (i = 0; i < arg_no; i++)
		  printf("arg[%d] = %s\n", i, arg[i]);

		promptNotRun = 0;// flag for printing prompt
		builtInRun = 0;
		
		// built in commands
                if (strcmp(arg[0], "pwd") == 0) { // built-in command pwd 
		  printf("Executing built-in [pwd]\n");
		  builtInRun = 1;
	          ptr = getcwd(NULL, 0);
                  printf("%s\n", ptr);
                  free(ptr);
	        }//if for pwd
		else
                if (strcmp(arg[0], "which") == 0) { // built-in command which
		  struct pathelement *p, *tmp;
                  char *cmd;

		  p = get_path();
		  tmp = p;
		  while (tmp) {      // print list of paths
		    printf("path [%s]\n", tmp->element);
		    tmp = tmp->next;
                  }//while
                    
                  cmd = which(arg[1], p);
		  builtInRun = 1;
                  if (cmd) {
		    printf("Executing built-in [which]\n");
		    printf("%s\n", cmd);
                    free(cmd);
                  }//if
		  else
		    printf("command %s not found\n", arg[1]);

		  while (p) {   // free path list
		     tmp = p;
		     p = p->next;
		     free(tmp->element);
		     free(tmp);
				}//while
	        }//if
		if (strcmp(arg[0], "where") == 0){ // built-in command where
			struct pathelement *path, *temp;

			path = get_path();
			temp = path;
			
			while(temp){
				printf("path [%s]\n", temp->element);
				temp = temp->next;
			}
			printf("Executing built-in [where]\n");
			where(arg[1], path);
			builtInRun = 1;
		}//where
		if (strcmp(arg[0], "cd") == 0){// built-in command cd
			builtInRun = 1;
			if (arg[1] != NULL && arg[2] != NULL){
				printf("cd:\tToo many arguments\n");
			}//if
			else{
				printf("Executing built-in [cd]\n");
				cd(arg[1]);
			}//else
		}//cd
		if (strcmp(arg[0], "pid") == 0){//built-in command pid
			builtInRun = 1;
			printf("Executing built-in [pid]\n");
			pid = getpid();
			printf("Shell PID: %d\n",pid);
		}//pid
		if (strcmp(arg[0], "printenv") == 0){//built-in command printenv
			builtInRun = 1;
			printf("Executing built-in [printenv]\n");
			if(arg[1] != NULL){// one arg for printenv
				printenv(arg[1]);
			}//if
			else{//print entire enviorment
				printenv(NULL);
			}//if
		}//printenv
		if (strcmp(arg[0], "kill") == 0){// built-in command kill
			builtInRun = 1;
			printf("Executing built-in [kill]\n");
			int pid, worked;// pid of process to kill, if kill worked
			int elseStop = 0;
			if(arg[1] != NULL && arg[2] != NULL && arg[3] != NULL){// if too many args
				printf("%s:  too many arguments\n",arg[0]);
				worked = 1;
				elseStop = 1;
			}//if
			if(arg[1] != NULL && arg[2] == NULL){// kill signal given, no extra information
				pid = atoi(arg[1]);
				worked = killProc(pid,NULL);
				elseStop = 1;
			}//if
			if(arg[1] != NULL && arg[2] != NULL){//given signal command
				pid = atoi(arg[2]);
				worked = killProc(pid,arg[1]);
				elseStop = 1;
			}//if
			if(elseStop == 0){
				pid = getpid();
				worked = killProc(pid,NULL);
			}//else

			//if kill worked or not
			if(worked == -1){// no exististing PID of that kind
				printf("PID %d does not exist\n",pid);
			}//if
			if(worked == 0){
				printf("Killed process: %d\n",pid);
			}//if
			if(worked == 1){
				printf("%s did not stop any process: too many arguments\n",arg[0]);
			}//else
		}//kill
		if (strcmp(arg[0], "list") == 0){//built-in command list
			builtInRun = 1;
			printf("Executing built-in [list]\n");
			if(arg[1] == NULL){
				list(NULL);
			}//if
			else{
				for(int i=1; arg[i] != NULL; i++){
					list(arg[i]);
				}//for
			}//else
		}//list
		if (strcmp(arg[0], "prompt") == 0){//built-in command prompt
			builtInRun = 1;
			printf("Executing built-in [prompt]\n");
			int worked = 0;
			if(arg[1] != NULL && arg[2] == NULL){
				free(newPrompt); // free old prompt
				newPrompt = prompt(arg[1]);
				worked = 1;
				promptNotRun = 1;
			}//if
			if(arg[1] == NULL){
				printf("Enter a new prompt:");
				if(fgets(buf, MAXLINE, stdin) != NULL){
                			if (buf[strlen(buf) - 1] == '\n'){
                        			buf[strlen(buf) - 1] = 0; /* replace newline with null */
					}//if
				}//if
				prompt(buf);
				worked = 1;
				promptNotRun = 1;
			}//if
			if(worked == 0){
				printf("%s: Too many arguments\n",arg[0]);
			}//if
		}//prompt
		if (strcmp(arg[0], "setenv") == 0){//built-in command setenv
			builtInRun = 1;
			printf("Executing built-in [setenv]\n");
			if(arg[1] == NULL){//prints enviorment
				setEnv(NULL,NULL);
			}//if
			if(arg[1] != NULL && arg[2] == NULL){
				setEnv(arg[1],NULL);
			}//if
			if(arg[1] != NULL && arg[2] != NULL){
				setEnv(arg[1],arg[2]);
			}//if
			if(arg[3] != NULL){
				printf("%s: Too many arguments\n",arg[0]);
			}//if
		}//setenv
		if (strcmp(arg[0], "exit") == 0){//built-in command exit
			printf("Executing built-in [exit]\n");
			exit(0);
		}//exit
		else {
		    if(builtInRun == 0){
			//print out path of executable
			char *filePath;
			struct pathelement *exePath, *tmp;
			exePath = get_path();
			filePath = which(buf,exePath);
			printf("Executing [%s]\n",filePath); // printing filepath
			free(filePath);
			while (exePath) {   // free path list
				tmp = exePath;
				exePath = exePath->next;
				free(tmp->element);
				free(tmp);
			}//while
		    }//if
		  if ((pid = fork()) < 0) {
			printf("fork error");
		  } else if (pid == 0 && strcmp(arg[0],"pid") != 0 && strcmp(arg[0],"cd") != 0
			&& strcmp(arg[0],"printenv") != 0 && strcmp(arg[0],"kill") != 0
			&& strcmp(arg[0],"list") != 0 && strcmp(arg[0],"prompt") != 0
			&& strcmp(arg[0],"where") != 0 && strcmp(arg[0],"setenv") != 0 
			&& strcmp(arg[0],"pwd") != 0){/* child */			
			execlp(buf, buf, (char *)0);
			printf("couldn't execute: %s\n", buf);
			exit(127);
		  }

		  /* parent */
		  if ((pid = waitpid(pid, &status, 0)) < 0 && strcmp(arg[0],"pid") != 0 && strcmp(arg[0],"cd") != 0
			&& strcmp(arg[0],"printenv") != 0 && strcmp(arg[0],"kill") != 0 && strcmp(arg[0],"list") != 0
			&& strcmp(arg[0],"prompt") != 0 && strcmp(arg[0],"where") != 0 && 
			strcmp(arg[0],"setenv") != 0 && strcmp(arg[0],"pwd") != 0)
			printf("waitpid error\n");
/**
                  if (WIFEXITED(status)) S&R p. 239 
                    printf("child terminates with (%d)\n", WEXITSTATUS(status));
**/
                }
	reprompt:
		if(promptNotRun == 0){
			cwd = getcwd(NULL,0);
			printf("[%s]$ ",cwd);
			free(cwd);
		}//if
		else{
			printf("%s$ ",newPrompt);
		}//else
	}
	exit(0);
}
