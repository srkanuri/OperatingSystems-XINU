#include <xinu.h>
#include <prodcons.h>
/***************************************************************
 * consume.c - Program which consumes the resource n until count
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 09/20/2016
 * Last Modified by: Mangarish Wagle
 * Date Last Modified: 09/27/2016
 * Assignment: 2 and 3
 ***************************************************************/

extern int n;
extern sid32 produced, consumed;

//Code to consume values of global variable 'n' until the value
//of n is zero.
//print consumed value e.g. consumed : 8
void consumer(int count) {
  int32 p = 0;
  while (1){
    if(n > 0) {
      wait(produced);
      printf("Consumed> Total consumed: %d\tResources available: %d\n", ++p, --n);
      signal(consumed);
    }
  }
}

