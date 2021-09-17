# HomeworkAssignment-Semaphores
Individual homework assignment in dealing with semaphores

Editors: Malachi Parks
Section: CISC361-010
Assignment: Homework Assignment: Semaphores
Due Date: 11/17/2020
Makefile Notes
	make all		makes all executables
	make fork-join		makes fork-join executable
	make rendezvous		makes rendezvous executable
	make order		makes order executable

---------------------------------------------------------------------------------------

Fork-join.c Notes

semaphore is init to 0 since when it is decremented by sem_wait does not
return until the s > -1.

https://www.man7.org/linux/man-pages/man3/sem_init.3.html (man page of init)

---------------------------------------------------------------------------------------

Rendezvous.c Notes
Had to decrement one, wait for the other child to increment first child then keep going

---------------------------------------------------------------------------------------

Order.c	 Notes
Worked backward with locks, notes in .c file on how to implement
