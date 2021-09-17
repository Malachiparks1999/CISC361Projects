CC=gcc
# CC=gcc -Wall

mysh: get_path.o which.o shell-with-builtin.o createPipe.o cd.o
	$(CC) -pthread -g shell-with-builtin.c get_path.o which.o createPipe.o cd.o -o mysh

shell-with-builtin.o: shell-with-builtin.c sh.h
	$(CC) -g -c shell-with-builtin.c

get_path.o: get_path.c get_path.h
	$(CC) -g -c get_path.c

which.o: which.c get_path.h
	$(CC) -g -c which.c

createPipe.o: createPipe.c
	$(CC) -g -c createPipe.c

cd.o:	cd.c
	$(CC) -g -c cd.c

clean:
	rm -rf *.o mysh
