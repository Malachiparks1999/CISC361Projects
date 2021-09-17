#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 128
#define MAXARGS 16

void parse_commandline(char *line)
{
  int i, j;
  char *p;
  char **args = calloc(MAXARGS, sizeof(char*));

  i = 0;
  p = strtok(line, " "); 	
  do {				
    // args[i] = p;
    // i++; // i = i + 1;
    args[i++] = p;
  } while (p = strtok(NULL, " "));

  for (j = 0; j < i; j++)
    printf("args (%d) is [%s]\n", j, args[j]);
}

int main(void)
{
  char	buf[MAXLINE];	
  pid_t	pid;
  int	status;
  
  printf("> ");  /* print prompt */
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      buf[strlen(buf) - 1] = 0; /* replace newline with null */

    printf("[%s]\n", buf);
    parse_commandline(buf);
    printf("> ");
  }
  exit(0);
}
