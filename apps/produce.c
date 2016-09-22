#include <xinu.h>
#include <prodcons.h>
/***************************************************************
 * produce.c - Program which produces the resource n until count
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 09/20/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 09/20/2016
 * Assignment: 2
 ***************************************************************/

extern int n;

//Code to produce values less than equal to count, 
//produced value should get assigned to global variable 'n'.
//print produced value e.g. produced : 8
void producer(int count) {
  int cnt=0;
  
  while (1) {
    if(n < count) {
      printf("produced:\t%d\n", ++n);
    }
    else {
      sleep(10);
      continue;
    }
  }
}    

