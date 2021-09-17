/* strtok example */
/* http://www.cplusplus.com/reference/cstring/strtok/ */
/*
Splitting string "- This, a sample string." into tokens:
starting address of str[] = 0x7fff3cef6af0
0x7fff3cef6af2 This
0x7fff3cef6af8 a
0x7fff3cef6afa sample
0x7fff3cef6b01 string
*/
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] = "- This, a sample string.";
  char *pch;

  printf("Splitting string \"%s\" into tokens:\n", str);
  printf("starting address of str[] = %p\n", str);

  pch = strtok(str, " ,.-");
  while (pch != NULL)
  {
    printf ("address = %p -- [%s]\n", pch, pch);
    pch = strtok(NULL, " ,.-");
  }

  printf("The original string became [%s]\n", str);
  return 0;
}
