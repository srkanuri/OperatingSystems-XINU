#include <xinu.h>
#include <prodcons.h>
/***************************************************************
 * consume.c - Program which consumes the resource n until count
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 09/20/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 09/20/2016
 * Assignment: 2
 ***************************************************************/

extern int n;

//Code to consume values of global variable 'n' until the value
//of n is zero.
//print consumed value e.g. consumed : 8
void consumer(int count) {
  while (1){
    if(n > 0) {
      printf("consumed:\t%d\n", --n);
    }
    else{
      sleep(1);
      continue;
    } 
  }
}

