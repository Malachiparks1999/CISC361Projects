/***
char *getcwd(char *buf, size_t size);

As  an  extension  to the POSIX.1-2001 standard, glibc's getcwd() allo‐
cates the buffer dynamically using malloc(3) if buf is NULL.   In  this
case,  the  allocated  buffer  has the length size unless size is zero,
when buf is allocated as big as necessary.  The caller  should  free(3)
the returned buffer.
***/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  char *ptr;

  ptr = getcwd(NULL,0);

  printf("[%s]\n", ptr);

  free(ptr);
}
