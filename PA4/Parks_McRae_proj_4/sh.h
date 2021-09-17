/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 4
Due Date: 10/29/2020
*/

#include "get_path.h"

int pid;
char *which(char *command, struct pathelement *pathlist);
void list(char *dir);
void printenv(char **envp);
void createPipe(char ** inputArgs);
void createPipeErr(char **inputArgs);
void cd(char *dir);

#define PROMPTMAX 64
#define MAXARGS   16
#define MAXLINE   128
