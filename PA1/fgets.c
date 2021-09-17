#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFERSIZE 128

// an empty string terminates the program

int main (int argc, char *argv[])
{
  char buffer[BUFFERSIZE];
  printf("Enter a message: \n");
  while((fgets(buffer, BUFFERSIZE , stdin) != NULL) &&
        (buffer[0] != '\n')) // empty string ???
  {
    printf("%s", buffer);
  }
  return 0;
}
