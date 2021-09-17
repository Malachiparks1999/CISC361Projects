CC=gcc
# CC=gcc -Wall

all: fork-join rendezvous order

fork-join: fork-join.o
	$(CC) -pthread -g fork-join.o -o fork-join

rendezvous: rendezvous.o
	$(CC) -pthread -g rendezvous.o -o rendezvous

order: order.o
	$(CC) -pthread -g order.o -o order

fork-join.o: fork-join.c
	$(CC) -g -c fork-join.c

rendezvous.o: rendezvous.c
	$(CC) -g -c rendezvous.c

order.o: order.c
	$(CC) -g -c order.c

clean:
	rm -rf *.o fork-join rendezvous order
