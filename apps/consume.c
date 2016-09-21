#include <xinu.h>
#include <prodcons.h>
/***************************************************************
 * consume.c - Program which consumes the resource n until count
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangarish Wagle (mawagle)
 * Date Created: 09/20/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 09/20/2016
 * Assignment: 2
 ***************************************************************/

//Code to consume values of global variable 'n' until the value
//of n is less than or equal to count
//print consumed value e.g. consumed : 8
void consumer(int count) {
  int cnt=0;
  while (cnt<count){
    if(n!=0){
      printf("consumed:\t%d\n", --n);
      cnt++;
    }
    else{
      sleep(10);
      continue;
    } 
  }
}

