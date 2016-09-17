/* xsh_hello.c - hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - Custom code implementation of hello command
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {

  int32	retval;			/* return value			*/
  uint32	now;			/* current local time		*/
  char	datestr[64];		/* printable date in ascii	*/
  
  /* Output info for '--help' argument */
  
  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
    printf("Usage: %s\n\n", args[0]);
    printf("Description:\n");
    printf("\tHello command to welcome you into the world of Xinu.\n");
    return 0;
  }
  
  /* Check argument count */
  
  if (nargs > 2) {
    fprintf(stderr, "%s: too many arguments\n", args[0]);
	  fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
	  return 1;
	}
  
  if (nargs == 2) {
    printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
    return 0;
  }
  
  printf("Welcome to Xinu\n");
  return 0;
}
