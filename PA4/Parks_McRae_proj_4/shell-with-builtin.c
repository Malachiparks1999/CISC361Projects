/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 4
Due Date: 10/29/2020
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <glob.h>
#include <sys/wait.h>
#include <utmpx.h>
#include <pthread.h>
#include "sh.h"

// WatchUser thread and lock creation
pthread_mutex_t collisionLock = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread;

// WatchUser Singly Linked List Functions
struct node{
        char*   username;       //username entered when watch user starts
        struct node *next;
};

// Global SLL Vars
struct node *head = NULL;
struct node *current = NULL;

void displayUserList(){
        struct node *tmp = head; // creating node tmp to traverse list
        while(tmp){ // traversing the list and printing out username till tmp is NULL
                printf("[%s] ",tmp->username);
                tmp=tmp->next;
        }
	printf("\n");
}//displayUserList

int listEmpty(){//checks if list is empty
        return head == NULL;
}//empty

int userCount(){        // used to count the length of the SLL
        int userCount = 0;
        struct node *tmp;

        tmp = head; // use to traverse the list

        while(tmp){ // if pointer not null keep counting users
                userCount++;
                tmp=tmp->next;
        }
        return userCount;
}// userCount

void delete(char* givenUsername){
    struct node **tmp, *erase;//temp is used to traverse, erase holds the value to be erased
    tmp=&head;

    while(*tmp != NULL){
        if((*tmp)->username == givenUsername){
            erase = *tmp; // used to hold value to be erased, erase did not like being a pptr, works file with ptr
            *tmp = (*tmp)->next; // remove node
            free(erase);//free old memory
        }//if
        else{
            tmp = &((*tmp)->next); // move on
        }//else
    }//while
}//delete

void addAtFront(char* givenUsername){           //Adds new username to the front of the SLL
        struct node *newFront = malloc(sizeof(struct node)); // creating new node which will be added to front
        newFront->username = givenUsername;     //setting username field to passed in name
        newFront->next = head;          // Adding to front of list
        head = newFront;        //resetting head
}//addAtFront

// Function for multi thread where it's logging Users
void *userWatching(void *arg){
        struct utmpx *currentUser; //struct holding information about who is logged in on a terminal
        setutxent();    //sets thread for currentUser

        int timewatching = 30;          //time remaing before the watch user just automatically stops
        while(timewatching > 0){        //time hasn't run out on watching user
                pthread_mutex_lock(&collisionLock);             //prevent both threads from accessing SLL at once

                while(currentUser = getutxent()) {     //returns not null if able to pull continuous users
                        if(currentUser->ut_type == USER_PROCESS){      // normal process init by user

                                struct node* tmp = head;
                                while(tmp){     //while not null search through LL
                                        if(currentUser->ut_user == tmp->username){
                                                printf("%s has logged on %s from %s\n", currentUser->ut_user,
                                                currentUser->ut_line,currentUser->ut_host);
                                        }
                                        tmp = tmp->next;
                                }
                        }
                }

        pthread_mutex_unlock(&collisionLock);            //unlocking so watchuser function can access SLL now
        timewatching = timewatching - 3;
        sleep(3);
    }
}

// Creation of multi-threads

void watchUser(char **cmdArgs){
	if(listEmpty()) {	// if list is empty insert a buffer user
		addAtFront("");
	}

	printf("UserList Before:");
	displayUserList();	// show current User List to stdout

	if(thread == NULL) {	// if thread doesn't exist make it once
		pthread_create(&thread, NULL, userWatching,(void *) cmdArgs[1]); 	//stores thread in address of it, uses Userwatching to mark users
		printf("Created new watchuser thread\n");
	}

	pthread_mutex_lock(&collisionLock);		// prevent SLL from being double accessed by threads

	if (cmdArgs[2] != NULL && strcmp(cmdArgs[2], "off") == 0) {	// turn off watchuser (deleteing user from SLL)
		delete(cmdArgs[1]);
	}
	else{	// watch a new user by adding to SLL
		printf("Started watching user: %s\n",cmdArgs[1]);
		addAtFront(cmdArgs[1]);
	}

	printf("UserList After:");
	displayUserList();	// Once done show user list

	pthread_mutex_unlock(&collisionLock);
}//watchUser

// Sig handler for ctrl C
void sig_handler(int sig)
{
  fprintf(stdout, "\n>> ");
  fflush(stdout);
}
  
int
main(int argc, char **argv, char **envp)
{
	char	buf[MAXLINE];
	char    *arg[MAXARGS];  // an array of tokens
	char    *ptr;
        char    *pch;
	pid_t	pid;
	int	status, i, arg_no, background, noclobber;
	int     redirection, append, pipes, rstdin, rstdout, rstderr;
        struct pathelement *dir_list, *tmp;
        char *cmd;

        noclobber = 0;         // initially default to 0

        signal(SIGINT, sig_handler);

	fprintf(stdout, ">> ");	/* print prompt (printf requires %% to print %) */
	fflush(stdout);
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (strlen(buf) == 1 && buf[strlen(buf) - 1] == '\n')
		  goto nextprompt;  // "empty" command line

		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0; /* replace newline with null */

	                       // no redirection or pipe
	        redirection = append = pipes = rstdin = rstdout = rstderr = 0; 
                // check for >, >&, >>, >>&, <, |, and |&
		if (strstr(buf, ">>&")) 
		  redirection = append = rstdout = rstderr = 1;
		else
		if (strstr(buf, ">>")) 
		  redirection = append = rstdout = 1;
		else
		if (strstr(buf, ">&")) 
		  redirection = rstdout = rstderr = 1;
		else
		if (strstr(buf, ">")) 
		  redirection = rstdout = 1;
		else
		if (strstr(buf, "<")) 
		  redirection = rstdin = 1;
		else
		if (strstr(buf, "|&")) 
		  pipes = rstdout = rstderr = 1;
		else
		if (strstr(buf, "|")) 
		  pipes = rstdout = 1;

           /***
                printf("-%d-%d-%d-%d-%d-%d-\n",
		       redirection, append, pipe, rstdin, rstdout, rstderr);
           ***/

		// parse command line into tokens (stored in buf)
		arg_no = 0;
                pch = strtok(buf, " ");
                while (pch != NULL && arg_no < MAXARGS)
                {
		  arg[arg_no] = pch;
		  arg_no++;
                  pch = strtok (NULL, " ");
                }
		arg[arg_no] = (char *) NULL;

		if (arg[0] == NULL)  // "blank" command line
		  goto nextprompt;

                background = 0;      // not background process
		if (arg[arg_no-1][0] == '&')
		  background = 1;    // to background this command

		// print tokens of this command line
            /***
		for (i = 0; i < arg_no; i++)
		  printf("arg[%d] = %s\n", i, arg[i]);
            ***/

                if (strcmp(arg[0], "exit") == 0) { // built-in command exit
		  printf("Executing built-in [exit]\n");
                  exit(0);
	        }
		else
                if (strcmp(arg[0], "pwd") == 0) { // built-in command pwd 
		  printf("Executing built-in [pwd]\n");
	          ptr = getcwd(NULL, 0);
                  if (redirection) {
		    int fid, used;
		    if (!append && rstdout && !rstderr && !noclobber){  // ">, no clobber is 0 so allow overwrite"
                      fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP);
		    } 
		    if(!append && rstdout && !rstderr && noclobber){    // ">, clobber is 1 so doesn't overwrite and prints out error to stdout
		      fprintf(stdout, "File already exists\n");
                      fflush(stdout);
		      goto nextprompt;
		    } 
		    if (!append && rstdout && rstderr && !noclobber){ // ">&, noclobber is 0 so allow creation"
		      fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP);
		      close(1);
		      close(2);
                      dup(fid);
		      dup(fid);
                      close(fid);
                      printf("%s\n", ptr);  // redirect stdout to file
                      fid = open("/dev/tty", O_WRONLY); // redirect stdout
                      close(1);                         // back to terminal
                      close(2);
		      dup(fid);
		      dup(fid);
                      close(fid);
		      goto nextprompt;
                    } 
		    if (!append && rstdout && rstderr && noclobber){// ">&, noclobber is 0 so no overwrite"
		      fprintf(stdout, "File already exists\n");
		      fflush(stdout);
		      goto nextprompt;
		    }
		    if (append && rstdout && !rstderr && !noclobber){   // ">>, no clobber is 0 so allow creation"
                      fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP);
		    } 
		    if (append && rstdout && !rstderr && noclobber){    // ">>, clobber is 1 so can't create
		      fprintf(stdout, "Can't create a new file\n");
		      fflush(stdout);
		      goto nextprompt;
		    }
		    if (append && rstdout && rstderr && !noclobber){	// ">>&, clober is 0 so can append
		      fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP);
                      close(1);
                      close(2);
                      dup(fid);
                      dup(fid);
                      close(fid);
                      printf("%s\n", ptr);  // redirect stdout to file
                      fid = open("/dev/tty", O_WRONLY); // redirect stdout
                      close(1);                         // back to terminal
                      close(2);
                      dup(fid);
                      dup(fid);
                      close(fid);
                      goto nextprompt;
		    }
		    if (append && rstdout && rstderr && noclobber){	// ">>&, clober is 1 so can't create new file"
		      fprintf(stdout, "Can't create a new file\n");
		      fflush(stdout);
		      goto nextprompt;
		    }

                    close(1);
                    dup(fid);
                    close(fid);
                    printf("%s\n", ptr);  // redirect stdout to file
		    fid = open("/dev/tty", O_WRONLY); // redirect stdout
                    close(1);                         // back to terminal
                    dup(fid);
                    close(fid);
		    
                  } else
                    printf("%s\n", ptr);  // no redirection
                  free(ptr);
	        }
		else
                if (strcmp(arg[0], "noclobber") == 0) { // built-in command noclobber
		  printf("Executing built-in [noclobber]\n");
		  noclobber = 1 - noclobber; // switch value
		  printf("%d\n", noclobber);
	        }
		else
                if (strcmp(arg[0], "echo") == 0) { // built-in command echo
		  int fid;
		  printf("Executing built-in [echo]\n");
		  if(redirection){// there is basic redirection
		    if (!append && rstdout && !rstderr && !noclobber){  // ">, no clobber is 0 so allow overwrite"
                      fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP);
                    }
                    if(!append && rstdout && !rstderr && noclobber){    // ">, clobber is 1 so doesn't overwrite and prints out error to stdout
                      fprintf(stdout, "File already exists\n");
                      fflush(stdout);
                      goto nextprompt;
                    }
		    if (append && rstdout && !rstderr && !noclobber){   // ">>, no clobber is 0 so allow creation"
                      fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP);
                    }
                    if (append && rstdout && !rstderr && noclobber){    // ">>, clobber is 1 so can't create
                      fprintf(stdout, "Can't create a new file\n");
                      fflush(stdout);
                      goto nextprompt;
                    }
		    close(1);
                    dup(fid);
                    close(fid);
		    for (i = 0; i < arg_no; i++)
		      printf("arg[%d] = %s\n", i, arg[i]); // print out echo to stdout
		    fid = open("/dev/tty", O_WRONLY);
		    close(1);			      // back to terminal
		    dup(fid);
		    close(fid);
		  } else{ // no redirection
			for(i = 0; i< arg_no; i++)
			  printf("arg[%d] = %s\n", i, arg[i]);
		    }
	        }
		else
                if (strcmp(arg[0], "which") == 0) { // built-in command which
		  printf("Executing built-in [which]\n");

		  if (arg[1] == NULL) {  // "empty" which
		    printf("which: Too few arguments.\n");
		    goto nextprompt;
                  }

		  dir_list = get_path();

                  cmd = which(arg[1], dir_list);
                  if (cmd) {
		    printf("%s\n", cmd);
                    free(cmd);
                  }
		  else               // argument not found
		    printf("%s: Command not found\n", arg[1]);

		  while (dir_list) {   // free list of path values
		     tmp = dir_list;
		     dir_list = dir_list->next;
		     free(tmp->element);
		     free(tmp);
                  }
	        }
		if (strcmp(arg[0], "watchuser") == 0) {  // built-in command which
		     printf("Executing built-in [watchuser]\n");
		     watchUser(arg);
		}
		if (strcmp(arg[0], "pid") == 0) { // built-in command which
                	printf("Executing builtin: pid\n");
                	int pid = getpid();
                	printf("%d\n",pid);
        	}
		if (strcmp(arg[0], "cd") == 0){// built-in command cd
                     	if (arg[1] != NULL && arg[2] != NULL){
                                printf("cd:\tToo many arguments\n");
                        }//if
                        else{
                                printf("Executing built-in [cd]\n");
                                cd(arg[1]);
                        }//else
                }//cd
                // add other built-in commands here
		else {  // external command
		  if ((pid = fork()) < 0) {
			printf("fork error");
		  } else if (pid == 0) {		/* child */
			                // an array of aguments for execve()
	                char    *execargs[MAXARGS]; 
		        glob_t  paths;
                        int     csource, j;
			char    **p;
			
			/* Piping */
			if(pipe){
				if(rstdout && !rstderr){	//normal pipe
					createPipe(arg);
				}
				else if(rstdout && rstderr){		// stderr piped as well
					createPipeErr(arg);
				}
			}//pipe

			/* File Redirection */
			if(redirection){ // if there is a redirection
			  int fid;
			  if (!append && rstdout && !rstderr && !noclobber){ 		// ">, with no clobber being 0 allows for error"
				close(1);	//close stdout
				fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP);		// new fd pointing to args
				dup(fid);	//fid now pointed to stdout
			  }
			  if (!append && rstdout && !rstderr && noclobber){		// ">, with clobber being 0 doesn't allow overwrite"
				fprintf(stdout,"File already exists\n");
				fflush(stdout);
				exit(1);
			  }
			  if (!append && rstdout && rstderr && !noclobber){		// ">&, with clobber being 0 can append
				close(1);	//close stdout
				close(2);	//close stderr
				fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP);
				dup(fid);	// fid now pointed to stdout
				dup(fid);	//fid now pointed to as stderr
			  }
			  if (!append && rstdout && rstderr && noclobber){		// ">&, with clobber being 1 cannot append"
				fprintf(stdout,"File already exists\n");
				fflush(stdout);
				exit(1);
			  }
			  if (append && rstdout && !rstderr && !noclobber){		// ">>" with clobber being 0 so can create new
				close(1);	//close stdout
				fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP);
				dup(fid);
			  }
			  if (append && rstdout && !rstderr && noclobber){		// ">>" with clobber being 1 so can't create new
				fprintf(stdout, "Cannot create new file\n");
				fflush(stdout);
				exit(1);
			  }
			  if (append && rstdout && rstderr && !noclobber){		// ">>&", with clobber 0 can create
				close(1);	//close stdout
				close(2);	//close stderr
				fid = open(arg[arg_no-1], O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP);
				dup(fid);
				dup(fid);
			  }
			  if (append && rstdout && rstderr && noclobber){              // ">>&", with clobber 0 cannot create new file
                                fprintf(stdout, "Cannot create new file\n");
				fflush(stdout);
				exit(0);
                          }
			  if (stdin){		//"<" which reads in the files input to a command
				close(0);
				fid = open(arg[arg_no-1], O_RDONLY, S_IRUSR|S_IWUSR|S_IRGRP);
				dup(fid);
			  }
			  // implement stdin
			}//redirection

                        if (arg[0][0] != '/' && strncmp(arg[0], "./", 2) != 0 && strncmp(arg[0], "../", 3) != 0) {  // get absoulte path of command
		          dir_list = get_path();      
                          cmd = which(arg[0], dir_list);
                          if (cmd) 
		            printf("Executing [%s]\n", cmd);
		          else {              // argument not found
		            printf("%s: Command not found\n", arg[1]);
			    goto nextprompt;
                          }

		          while (dir_list) {   // free list of path values
		             tmp = dir_list;
		             dir_list = dir_list->next;
		             free(tmp->element);
		             free(tmp);
                          }
			  execargs[0] = malloc(strlen(cmd)+1);
			  strcpy(execargs[0], cmd); // copy "absolute path"
			  free(cmd);
			}
			else {
			  execargs[0] = malloc(strlen(arg[0])+1);
			  strcpy(execargs[0], arg[0]); // copy "command"
                        }

		        j = 1;
		        for (i = 1; i < arg_no; i++) { // check arguments
			  if (strchr(arg[i], '*') != NULL) { // wildcard!
			    csource = glob(arg[i], 0, NULL, &paths);
                            if (csource == 0) {
                              for (p = paths.gl_pathv; *p != NULL; ++p) {
                                execargs[j] = malloc(strlen(*p)+1);
				strcpy(execargs[j], *p);
				j++;
                              }
                              globfree(&paths);
                            }
			    else
                            if (csource == GLOB_NOMATCH) {
                              execargs[j] = malloc(strlen(arg[i])+1);
			      strcpy(execargs[j], arg[i]);
			      j++;
			    }
                          }
			  else {
                            execargs[j] = malloc(strlen(arg[i])+1);
			    strcpy(execargs[j], arg[i]);
			    j++;
			  }
                        }
                        execargs[j] = NULL;

			if (background) { // get rid of argument "&"
			  j--;
			  free(execargs[j]);
                          execargs[j] = NULL;
                        }

		        // print arguments into execve()
                     /***
		        for (i = 0; i < j; i++)
		          printf("exec arg[%d] = %s\n", i, execargs[i]);
                     ***/

			execve(execargs[0], execargs, NULL);
			printf("couldn't execute: %s", buf);
			exit(127);
		  }

		  /* parent */
		  if (! background) { // foreground process
		    if ((pid = waitpid(pid, &status, 0)) < 0)
	     	      printf("waitpid error");
                  }
		  else {              // no waitpid if background
		    background = 0;
		    sleep(1);
		    waitpid(pid, &status, WNOHANG); // used to clean up immediate jobs
		    goto nextprompt;
                  }
/**
                  if (WIFEXITED(status)) S&R p. 239 
                    printf("child terminates with (%d)\n", WEXITSTATUS(status));
**/
                }

           nextprompt:
		waitpid(-1, &status, WNOHANG); // clean up lurking jobs
		fprintf(stdout, ">> ");
		fflush(stdout);
	}
	exit(0);
}
