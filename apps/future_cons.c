#include <xinu.h>
#include <future.h>
/***************************************************************
 * future_cons.c - Consumer with Futures
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 10/06/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 10/06/2016
 * Assignment: 4
 ***************************************************************/

uint future_cons(future *fut) {
  //printf("\n[Process: %d] Inside future_cons", getpid());
  int i, status;
  status = future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("[Process: %d] Consumed: %d\n", getpid(),  fut->value);
  return OK;
}
