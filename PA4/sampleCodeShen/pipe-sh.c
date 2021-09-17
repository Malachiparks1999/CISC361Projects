#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// execute command: ls | wc -l

int main(void)
{
  int	fd[2], no_children;
  pid_t	left, right;
  char *argv_l[] = {"/bin/ls", 0};       
  char *argv_r[] = {"/usr/bin/wc", "-l", 0};

  no_children = 0;
  pipe(fd);   // shell creates the pipe

  left = fork();        // create the left child for "ls"
  if (left > 0) {	// shell the parent
    no_children++;
    right = fork();     // create the right child for "wc -l"
    if (right > 0) {	// shell the parent
      no_children++;
      close(fd[0]);     // parent has nothing to do with the pipe now
      close(fd[1]);     // so it closes both file descriptors
      while (no_children--)  // make sure all children are waited for
        wait(NULL);          // so there are no zombies
    } else {		// right child 
      close(fd[1]);     // fd[1] of no-use
      close(0);  
      dup(fd[0]);       // redirect stdin from fd[0] of pipe
      close(fd[0]);  
      execve(argv_r[0], argv_r, NULL);
    }
  } else {		// left child 
    close(fd[0]);       // fd[0] of no use
    close(1);  
    dup(fd[1]);         // redirect stdout to fd[1] of pipe
    close(fd[1]);
    execve(argv_l[0], argv_l, NULL);
  }

  exit(0);
}
