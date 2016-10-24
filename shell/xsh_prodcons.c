#include <xinu.h>
#include <stdlib.h>
#include <string.h>
#include <future.h>
#include <prodcons.h>
/***************************************************************
 * xsh_prodcons.c - Custom implementation of prodcons command
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 10/06/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 10/06/2016
 * Assignment: 5,4,3,2
 ***************************************************************/

/*Global variable n will be on heap and accessible in consume and produce*/
int n;  //Definition for global variable 'n'
sid32 produced, consumed; //global definitions for produced and consumed

shellcmd xsh_prodcons(int nargs, char *args[])
{
  //Argument verifications and validations
  int count = 2000;             //local varible to hold count

  int usefuture = 0;
  
  // Initializing semaphores
  produced = semcreate(0);
  consumed = semcreate(1);
  
  /* Output info for '--help' argument */
  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
    printf("Usage: %s [<count> | -f]\n\n", args[0]);
    printf("Description:\n");
    printf("\tProdcons command is used to demo the producer and consumer problem\n");
    printf("\tThe command accepts only 1 optional argument 'count'\n");
    printf("\tSpecify -f option to use futures instead of semaphores.\n");
    return 0;
  }
  
  /* Wrong argument count */
  if (nargs > 2) {
    fprintf(stderr, "%s: too many arguments\n", args[0]);
    fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
    return 1;
  }

  /* argument count 
  //check args[1] == -f and arg[2] if present assign value to count and usefuture.
  if (nargs == 3) {
      fprintf(stderr, "%s: wrong argument enterted\n", args[0]);
      fprintf(stderr, "%s accepts flag -f\n", args[0]);
      return 1;
    }
  } 
  else*/  
  if (nargs == 2) {
    if(strcmp(args[1], "-f") == 0) {
      usefuture = 1;
    } else {
      count = atoi(args[1]);
    }
    if(count == 0 && args[1] != 0){
      fprintf(stderr, "%s: wrong argument enterted\n", args[0]);
      fprintf(stderr, "%s accepts only an integer argument or -f for futures\n", args[0]);
      return 1;
    }
  }
  
  if(usefuture) {
     future *f1, *f2, *f3;
 
     f1 = future_alloc(FUTURE_EXCLUSIVE);
     f2 = future_alloc(FUTURE_SHARED);
     f3 = future_alloc(FUTURE_QUEUE);
 
     /*//FUTURE_EXCLUSIVE
     resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
     resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
     */

     /*//FUTURE_SHARED
     resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
     resume( create(future_cons, 1024, 20, "fcons3", 1, f2) );
     resume( create(future_cons, 1024, 20, "fcons4", 1, f2) ); 
     resume( create(future_cons, 1024, 20, "fcons5", 1, f2) );
     resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );*/

     // FUTURE_QUEUE
     resume( create(future_cons, 1024, 20, "fcons6", 1, f3) );
     resume( create(future_cons, 1024, 20, "fcons7", 1, f3) );
     resume( create(future_cons, 1024, 20, "fcons7", 1, f3) );
     resume( create(future_cons, 1024, 20, "fcons7", 1, f3) );
     resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );
     resume( create(future_prod, 1024, 20, "fprod4", 1, f3) );
     resume( create(future_prod, 1024, 20, "fprod5", 1, f3) );
     resume( create(future_prod, 1024, 20, "fprod6", 1, f3) );

  } else {
     /* Zero argument count */
     //create the process producer and consumer and put them in ready queue.
     //Look at the definations of function create and resume in the system folder for reference.      
     resume( create(producer, 1024, 20, "producer", 1, count));
     resume( create(consumer, 1024, 20, "consumer", 1, count));
  }
  sleep(2);
}
