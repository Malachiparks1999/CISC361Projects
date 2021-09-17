# choose your compiler 
CC=gcc 
#CC=gcc -Wall 

myshell: get_path.o linkedlist.o myshell.c 
	$(CC) -g myshell.c get_path.o -o myshell 

get_path.o: get_path.c get_path.h 
	$(CC) -g -c get_path.c 

linkedlist.o : linkedlist.c
	$(CC) -g -c linkedlist.c 	

clean: 
	rm -rf get_path.o linkedlist.o myshell