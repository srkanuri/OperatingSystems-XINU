#include <xinu.h>
#include <prodcons.h>
/***************************************************************
 * produce.c - Program which produces the resource n until count
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

//Code to produce values less than equal to count, 
//produced value should get assigned to global variable 'n'.
//print produced value e.g. produced : 8
void producer(int count) {

  // Limiting the number of produced items to count for demo.
  int32 x = 0;
  while(x<count) {
    // At any given time, produced items should not exceed count. 
    if(n < count) {
      wait(consumed);
      printf("Produced> Total produced: %d\tResources Available: %d\n",++x,++n);
      signal(produced);
    }
    else {
      sleep(1);
      continue;
    }
  }
}    

