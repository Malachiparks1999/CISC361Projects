all: tasty

tasty: tastylist_Parks_McRae.o
	gcc -g tastylist_Parks_McRae.o -o tasty

tastylist_Parks_McRae.o: tastylist_Parks_McRae.c
			gcc -g -c tastylist_Parks_McRae.c

clean:
	\rm *.o tasty
