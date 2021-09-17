// z.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void main()
{ 
  int i = 0, stat;
  pid_t pid, child;

  pid = fork();
  if (pid == 0) {   // CHILD
    printf("child [z] starts (for 10 secs)...\n");
    sleep(10);
    char *argv[2] = {"slp", NULL};
    printf("child [z] changes to [slp]...\n");
    execve("./slp", argv, NULL); // slp child sleeps for 20 seconds
  }
  else {            // PARENT no wait
    while (++i < 45) {
      printf("parent is alive (%d)...\n", i);
      sleep(1);
    }
    child = waitpid(pid, &stat, 0);
    printf("child [%d] terminates with status (%d)\n",
           child, WEXITSTATUS(stat));
    while (++i < 55) {
      printf("parent is alive (%d)...\n", i);
      sleep(1);
    }
  }
}
