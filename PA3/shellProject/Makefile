CC=gcc
# CC=gcc -Wall

mysh: get_path_free.o which.o shell-with-builtin.o where.o cd.o printenv.o kill.o list.o prompt.o setenv.o signalHandler.o
	$(CC) -g shell-with-builtin.c get_path_free.o which.o where.o cd.o printenv.o kill.o list.o prompt.o setenv.o signalHandler.o -o mysh

shell-with-builtin.o: shell-with-builtin.c sh.h
	$(CC) -g -c shell-with-builtin.c

get_path_free.o: get_path_free.c get_path.h
	$(CC) -g -c get_path_free.c

which.o: which.c get_path.h
	$(CC) -g -c which.c

where.o: where.c get_path.h
	$(CC) -g -c where.c

cd.o: cd.c
	$(CC) -g -c cd.c

printenv.o: printenv.c
	$(CC) -g -c printenv.c

kill.o: kill.c
	$(CC) -g -c kill.c

list.o: list.c
	$(CC) -g -c list.c

prompt.o: prompt.c
	$(CC) -g -c prompt.c

setenv.o: setenv.c
	$(CC) -g -c setenv.c

signalHandler.o: signalHandler.c
	$(CC) -g -c signalHandler.c

clean:
	rm -rf *.o mysh
