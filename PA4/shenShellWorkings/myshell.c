/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 4
Due Date: 10/?/2020
*/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <pwd.h>
#include <dirent.h>
#include <signal.h>
#include <wordexp.h>
#include "get_path.h"
#include "linkedlist.c"


//GLOBAL VARIABLES
extern char **environ; //environment variable
char* curdir; //current directory
char *prompt; //prompt from shell
int num_commands = 0; // number of commands already typed in shell
char* oldpwd; //previous working directory
char* prefix; //prefix for prompt can be modified by user
node aliases; //list of aliases
node history; //list of history commands


//FUNCTION PROTOTYPES
void shell_loop(); //loops through the shell 
char* read_line(); //reads in input
char** parse_args(char* line); //parses arguments into char** using space as delimiter
int execute(char** args); //calls required function based on args[0]
int other_process(char **args); //function to fork
void sig_int(int signo); //SIG_INT dealing fucntion
void sig_chld(int signo); //deals with signal from child
void sig_stp(int signo); //deals with sig stop  
char *which(char *command, struct pathelement *pathlist); //function for builtin which
char *where(char *command, struct pathelement *pathlist); //function for builtin where
int cd(char **args); //function for builtin cd
int pwd(); //function for built in pwd
int list(char **args); //function for builtin ls
int mykill(char **args); //builtin in function for kill
int myprompt(char **args); //edits shell prompt 
int getenviron(char **args); //prints shell environment
int setenviron(char **args); //edit shell environment variable
int myhistory(char **args); //prints last n shell commands
node myalias(char **args); //builtin alias
char** wildcardfunc(char **args, int loc); //deals with * and ?


int main (int argc, char* argv[]) {
	
	oldpwd = getcwd(NULL, PATH_MAX+1);
	num_commands = 0;
	
	//call shell loop
	shell_loop();

	free(oldpwd);
	
	exit(0);
}

//Keeps calling functions to parse input and execute commands until user enters exit or command fails
void shell_loop(){
	char *line;
	char **args;
	int stat = 1;

	prompt = (char*)malloc(128*sizeof(char));
	prefix = (char*)malloc(64*sizeof(char));
	do {	
		//signal catching functions 
		if (signal(SIGINT, sig_int) == SIG_ERR)
			printf("signal error\n");
		if (signal(SIGTSTP, sig_stp) == SIG_ERR)
			printf("signal error\n");
				
		curdir = getcwd(NULL, PATH_MAX+1);
		printf("%s ", prefix);
		printf("[%s]> ", curdir);

		line = read_line(); //reads in input
		if (line != NULL) { //line is NULL when it encounter EOF
			history = add(history, line, NULL); //keeps track of history
			num_commands++;
			args = parse_args(line); //parses arguments of line 
			stat = execute(args); //calls execute with these arguments, returns 1 if success, 0 for exit
			free(line);
			free(args);		
		}
						
		
	}while (stat == 1); 

	free(prompt);
	free(prefix);
	free(curdir);
	
	for(int i =0; i<num_commands; i++)
		history = removelist(history);			
		//do something
}

//reads in input from command line
char* read_line(){
	char buffer[128];
	int len;
	char* result;

	if (fgets(buffer, 128 , stdin) != NULL)
  	{
    		len = (int) strlen(buffer);
    		buffer[len - 1] = '\0'; 
    		result = (char *) malloc(len);
    		strcpy(result, buffer);
  	}
	else if (fgets(buffer, 128 , stdin) == NULL){ //when we hit the EOF char due to Ctrl+D or newline
		result = NULL;
	}
	return result;
}


//converts char* to char** using space as delimiter
char** parse_args(char* line){
	int num, i;
	num = 1;
	for(i=0; line[i]!='\0'; i++) //finds number of arguments
		if (line[i] == ' ')
			num++;
		
	char **result;
	result = (char**)malloc((num+1)*sizeof(char*)); //allocates required space
	i = 0;
	char* token = strtok(line, " ");
	
	while (token != NULL) {
		result[i] = token;
		i ++;
		token = strtok(NULL, " "); //assigns each value
	}

	result[num] = NULL; //ends argument lith NULL
	return result;	
}

//uses args to decide which functions to call
int execute(char** args){ 
	if (args[0] == NULL) //empty command
		return 1;
	int i;
	
	//checks for built ins
	if (strcmp(args[0], "exit") == 0) {
		printf("Executing builtin: exit\n");
		return 0;
	}
	
	if (strcmp(args[0], "cd") == 0) {
		printf("Executing builtin: cd\n");
		return cd(args);
	}

	if (strcmp(args[0], "which") == 0) {
		printf("Executing builtin: which\n");
		struct pathelement *pathlist;
		pathlist = get_path();
		char* result = which(args[1], pathlist);
		if (result == NULL)
			printf("Command not found\n");
		else
			printf("%s\n",result);	
		free(pathlist);
		free(result);
		return 1;
	}

	if (strcmp(args[0], "where") == 0) {
		printf("Executing builtin: where\n");
		struct pathelement *pathlist;
		pathlist = get_path();
		char* result = where(args[1], pathlist);
		if (result == NULL)
			printf("Command not found\n");
		else
			printf("%s",result);	
		free(pathlist);
		free(result);
		return 1;
	}

	if (strcmp(args[0], "pwd") == 0) {
		printf("Executing builtin: pwd\n");
		return pwd();	
	}

	if (strcmp(args[0], "list") == 0) {
		printf("Executing builtin: list\n");
		return list(args);	
	}

	if (strcmp(args[0], "pid") == 0) {
		printf("Executing builtin: pid\n");
		int pid = (int) getpid();
		printf("%d\n",pid); 
		return 1;
	}	

	if (strcmp(args[0], "kill") == 0) {
		printf("Executing builtin: kill\n");
		return mykill(args);	
	}

	if (strcmp(args[0], "prompt") == 0) {
		printf("Executing builtin: prompt\n");
		return myprompt(args);	
	}

	if (strcmp(args[0], "printenv") == 0) {
		printf("Executing builtin: printenv\n");
		return getenviron(args);	
	}

	if (strcmp(args[0], "setenv") == 0) {
		printf("Executing builtin: setenv\n");
		return setenviron(args);	
	}

	if (strcmp(args[0], "history") == 0) {
		printf("Executing builtin: history\n");
		return myhistory(args);	
	}

	if (strcmp(args[0], "alias") == 0) {
		printf("Executing builtin: alias\n");
		aliases = myalias(args);	
		//printlist(aliases);
		return 1;
	}
	//checks for absolute path
	if (args[0][0] == '.' || args[0][0] == '/'){
		if (access(args[0], F_OK) == 0) {
			printf("Checking for absolute path\n");
			execvp(args[0], args);	
		}
			return 1;
	}

	//checks input for * or ? wildcards
	i = 1;
	while(args[i]!=NULL){
		if (strstr(args[1], "*") || strstr(args[1], "?")) {
			char** p = wildcardfunc(args, i);
			int stat = other_process(p);
			free(p);
			return stat;
		}
		i++;
	}

	//if not built-in, use execvp
	printf("Executing: %s\n", args[0]);
	return other_process(args); //fork stuff
}

//deals with * and ? by returning list of files in directory that match the wildcard
char** wildcardfunc(char **args, int loc){
	wordexp_t p;
	char **words;
	int i, j;
	char **newargs;

	wordexp(args[loc], &p, 0);
	words = p.we_wordv;
	newargs = (char**)malloc((loc + p.we_wordc + 1)*sizeof(char*));
	for(j = 0; j < loc; j++)
		newargs[j] = args[j];

	for(i = 0; i<p.we_wordc; i++) {
		int len = strlen(words[i]);
		newargs[j] = (char *) malloc(len*sizeof(char));
		strcpy(newargs[j], words[i]);
		j++;
	}
	newargs[j] = NULL;

	wordfree(&p);

	return newargs;
}

//prints last n shell commands
int myhistory(char **args){ 
	if (args[1] == NULL) //if no arguments, print last 10 commands
		printpartial(history, num_commands, 10);
	else {
		int n = atoi(args[1]);
		printpartial(history, num_commands, n);
	}
	return 1;
}

//my alias command: returns updated list of aliases
node myalias(char **args){
	if (args[1] == NULL) { //prints existing aliases if no argument
		printlist(aliases);
		return aliases;	
	} else if (args[1] != NULL) //adds alias if there is an argument
	{
		char** synon = &args[2];
		return add(aliases, args[1], synon);
		
	}
}

//built in kill command
int mykill(char **args) {
	if (args[1] == NULL)
		perror("kill");
	else if (args[2] == NULL) {
		int pid = atoi(args[1]);
		kill(pid, SIGTERM); 
	}
	else {
		int signo = atoi(args[1]);
		signo = signo*(-1);
		int pid = atoi(args[2]);
		kill(pid, signo);
	}
	return 1;
}

//prints shell environment
int getenviron(char **args){ 
	char *tmp = *environ;
	int i = 1;
	if (args[1] == NULL) {
		while(tmp){
			printf("%s\n", tmp);
			tmp = *(environ+i);
			i++;
		}
	} else if (args[2] == NULL) {
		char *p = getenv(args[1]);
		printf("%s\n", p); 
		free(p);
	} else
		printf("printenv: Too many arguments\n");
	return 1;
}

//updates environment variable if tehre is an argument, prints shell env otherwise
int setenviron(char **args){
	if (args[3] != NULL) {
		printf("Error! Too many arguments!\n");
		return 1;
	}
	if (args[1] == NULL)
		getenviron(args);
	else if (args[2] == NULL)
		setenv(args[1], "", 0);
	else if (args[2] != NULL)
		setenv(args[1], args[2], 1);

	return 1;
}

//updates prefix for shell prompt
int myprompt(char **args){

	if (args[1] != NULL)
		strcpy(prefix,args[1]);
	else {	
		printf("input prompt prefix:");
		char* input = read_line();
		strcpy(prefix,input);
	}

	return 1;
}

//change directory function
int cd(char **args)
{	if (args[2] != NULL) {
		printf("Error! Too many arguments\n");
		return 1;
	}
	if (args[1] != NULL){ //we have some argument
		if (strcmp(args[1],"-") == 0) {
			if (chdir(oldpwd) != 0)
				perror(args[1]);
		}
		else {
			oldpwd = getcwd(NULL, PATH_MAX+1);
			if (chdir(args[1]) != 0) 
				perror(args[1]);

		}
  	} 
	if (args[1] == NULL){
		char* homedir = getenv("HOME");
		oldpwd = getcwd(NULL, PATH_MAX+1);
		if (chdir(homedir) != 0) 
			perror(args[1]);
	}
	
	return 1;
}

//prints current workign directory
int pwd(){
	if (getcwd(NULL, PATH_MAX+1) == NULL)
		printf("error!\n");
	else {
		printf("%s\n", getcwd(NULL, PATH_MAX+1));
	}
	return 1;
}

//lists files in directory
int list(char **args){
	if (args[1] == NULL) { //if no arguments
		DIR* dir;
		struct dirent *pDirent;
		dir = opendir(getcwd(NULL, PATH_MAX+1)); //opens current directory

		while ((pDirent = readdir(dir)) != NULL) {
            		printf ("%s\n", pDirent->d_name); //lists contents of directory by looping through
        	}

		closedir(dir);
		return 1;
	} else { //if there is an argument
		int i = 1;
		while(args[i] != NULL) {		
			DIR* dir;
			struct dirent *pDirent;
			dir = opendir(args[i]); //opens given directory
			if (dir == NULL) //if given directory could not be opened or found 
				perror(args[i]);
			else {
				printf("\n%s:\n", args[i]);
				while ((pDirent = readdir(dir)) != NULL) { //loop through directory and list contents
            				printf ("[%s]\n", pDirent->d_name);
        			}
				closedir(dir); 
			}
			i++;
		}
		return 1;
	}
}
//looks for given command in path and returns first instance
char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
	char *cmd;
	cmd = (char *)malloc(64*sizeof(char));
	while (pathlist) {
		sprintf(cmd, "%s/%s", pathlist->element, command);
			if (access(cmd, X_OK) == 0) {    
				return cmd;
			} 
    		pathlist = pathlist->next;
	}	
	return NULL;
} 
//looks for given command in path and returns all found instances
char *where(char *command, struct pathelement *pathlist)
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
	char *cmd;
	cmd = (char *)malloc(64*sizeof(char));
	char *result;
	result = (char *)malloc(128*sizeof(char));
	while (pathlist) {
		sprintf(cmd, "%s/%s", pathlist->element, command);
			if (access(cmd, X_OK) == 0) {    
				strcat(result, cmd);
				strcat(result, "\n");
			} 
    		pathlist = pathlist->next;
	}	
	free(cmd);
	return result;
}
//executes commands using execve
int other_process(char **args){
	pid_t pid;
	int status;
	
	if (signal(SIGINT, sig_int) == SIG_ERR)
		printf("signal error\n");
	if (signal(SIGCHLD, sig_chld) == SIG_ERR) ;
		// printf("signal error\n");

	if ((pid = fork()) < 0) { //forking failed
		printf("fork error\n");
	} else if (pid == 0) {		//child
		execvp(args[0], args); //call with execvp with given arguments
		printf("couldn't execute: %s\n", args[0]); //should not reach this if successful
		exit(127);
	}

	//parent
	if ((pid = waitpid(pid, &status, 0)) < 0) 
		printf("waitpid error");
	
	return 1;
}
//signal catching function
void sig_int(int signo)
{
	printf("signal (%d)\n", signo); ; 
	if (signo == SIGINT)
		printf("caught SIGINT\n");
	fflush(stdout);
}
void sig_stp(int signo)
{
	printf("signal (%d)\n", signo); ; 
	if (signo == SIGTSTP)
		printf("caught SIGSTP\n");
	fflush(stdout);
}

void sig_chld(int signo)
{
	if (signo == SIGINT)
		printf("caught SIGINT\n");
	if (signo == SIGCHLD) ;
		// printf("caught SIGCHLD\n");
}

	
