# CISC361PA3

/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 3
Due Date: 10/4/2020
*/

Programming Assignment 3 for CISC351 is where we have to create a simple shell like the tcsh shell

Directions can be found at: https://www.eecis.udel.edu/~cshen/361/PA_3_Simple_Shell/


To do list:
1. Parse "tolkenize" the commands into argv (done)
2. Check if given commands are built in or external (done)
	Check if argv[0] is internal or external (done)
3. Check thorugh absolute paths to see if executable is found (done)
4. If neither then search through the PATH in shell until command is found (done)
5. Once found use execve to run invoke. Also use watipid to make sure parent waits (done)
6. Before executing command print out its working directory (done)
7. Handle wildcard chars using glob
8. Using ctrl-c ignored when just shell but kill process if detected
9. Use ctrl-d to kill process and but not exit out of shell
10. NO MEMORY LEAKS


Build-in Commands to support:
	exit (done)
	which - finds one instance of executable (done)
	where - finds all instances (done)
	cd (done)
	pwd (done)
	list (done)
	pid (done)
	kill (done)
	prompt (done)
	printenv (done)
	setenv (done)

