#include <xinu.h>
#include <future.h>
/***************************************************************
 * future_prod.c - Producer with Futures
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 10/06/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 10/06/2016
 * Assignment: 5,4
 ***************************************************************/

uint future_prod(future *fut) {
  int i, j;
  j = (int)getpid();
  for (i=0; i<1000; i++) {
    j += i;
  }
  //printf("[Process: %d] Produced: %d\n", getpid(), j);
  future_set(fut, &j);
  return OK;
}
