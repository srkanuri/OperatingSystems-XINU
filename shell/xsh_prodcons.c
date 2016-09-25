#include <xinu.h>
#include <stdlib.h>
#include <prodcons.h>
/***************************************************************
 * xsh_prodcons.c - Custom implementation of prodcons command
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 09/20/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 09/20/2016
 * Assignment: 2
 ***************************************************************/
/*Global variable n will be on heap and accessible in consume and produce*/
int n;  //Definition for global variable 'n'
sid32 produced, consumed;

shellcmd xsh_prodcons(int nargs, char *args[])
{
  //Argument verifications and validations
  int count = 2000;             //local varible to hold count
  
  // Initializing semaphores
  produced = semcreate(0);
  consumed = semcreate(1);  
  
  /* Output info for '--help' argument */
  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
    printf("Usage: %s\n\n", args[0]);
    printf("Description:\n");
    printf("\tProdcons command is used to demo the producer and consumer problem\n");
    printf("\tThe command accepts only 1 optional argument 'count'");
    return 0;
  }
  
  /* Wrong argument count */
  if (nargs > 2) {
    fprintf(stderr, "%s: too many arguments\n", args[0]);
    fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
    return 1;
  }

  /* One argument count */
  //check args[1] if present assign value to count
  if (nargs == 2) {
    count = atoi(args[1]);
    if(count == 0 && args[1] != 0){
      fprintf(stderr, "%s: wrong argument enterted\n", args[0]);
      fprintf(stderr, "%s accepts only an integer argument\n", args[0]);
      return 1;
    }
  }
  
  /* Zero argument count */
  //create the process producer and consumer and put them in ready queue.
  //Look at the definations of function create and resume in the system folder for reference.      
  resume( create(producer, 1024, 20, "producer", 1, count));
  resume( create(consumer, 1024, 20, "consumer", 1, count));
  return 0;
}
