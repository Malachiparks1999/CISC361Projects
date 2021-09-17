/*
** pipe3.c -- a smallest pipe example
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int pfds[2];

	pipe(pfds);

	if (!fork()) {    // child
		close(1);       /* close normal stdout */
		dup(pfds[1]);   /* make stdout same as pfds[1] */
		close(pfds[0]); /* child doesn't need 0 */
		execlp("ls", "ls", NULL);
	} else {          // parent
		close(0);       /* close normal stdin */
		dup(pfds[0]);   /* make stdin same as pfds[0] */
		close(pfds[1]); /* parent doesn't need 1 */
		execlp("wc", "wc", "-l", NULL);
	}

	return 0;
}
