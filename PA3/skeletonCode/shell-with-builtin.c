#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "sh.h"

int
main(int argc, char **argv, char **envp)
{
	char	buf[MAXLINE];
	char    *arg[MAXARGS];  // an array of tokens
	char    *ptr;
        char    *pch;
	pid_t	pid;
	int	status, i, arg_no;

	printf(">> ");	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
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

                if (strcmp(arg[0], "pwd") == 0) { // built-in command pwd 
		  printf("Executing built-in [pwd]\n");
	          ptr = getcwd(NULL, 0);
                  printf("%s\n", ptr);
                  free(ptr);
	        }
		else
                if (strcmp(arg[0], "which") == 0) { // built-in command which
		  struct pathelement *p, *tmp;
                  char *cmd;

		  p = get_path();
		  tmp = p;
		  while (tmp) {      // print list of paths
		    printf("path [%s]\n", tmp->element);
		    tmp = tmp->next;
                  }
                    
                  cmd = which(arg[1], p);
                  if (cmd) {
		    printf("Executing built-in [which]\n");
		    printf("%s\n", cmd);
                    free(cmd);
                  }
		  else
		    printf("command %s not found\n", arg[1]);

		  while (p) {   // free path list
		     tmp = p;
		     p = p->next;
		     free(tmp->element);
		     free(tmp);
                  }
	        } 
		else {
		  if ((pid = fork()) < 0) {
			printf("fork error");
		  } else if (pid == 0) {		/* child */
			execlp(buf, buf, (char *)0);
			printf("couldn't execute: %s", buf);
			exit(127);
		  }

		  /* parent */
		  if ((pid = waitpid(pid, &status, 0)) < 0)
			printf("waitpid error");
/**
                  if (WIFEXITED(status)) S&R p. 239 
                    printf("child terminates with (%d)\n", WEXITSTATUS(status));
**/
                }
		printf(">> ");
	}
	exit(0);
}
