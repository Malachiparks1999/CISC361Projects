/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/4/2020
*/

#include "get_path.h"

int pid;
char *which(char *command, struct pathelement *pathlist);
void list(char *dir);
void printenv(char *envp);
void where(char *command, struct pathelement *path);
void cd(char *dir);
int killProc(int sig, char *sigterm);
char *prompt(char *enter);
int setEnv(char *name, char *value);
void sigIntHandler(int sigint);

#define PROMPTMAX 64
#define MAXARGS   16
#define MAXLINE   128
