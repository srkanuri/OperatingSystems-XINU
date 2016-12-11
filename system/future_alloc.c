/* future_alloc.c - future_alloc */

#include <xinu.h>
#include <future.h>
/***************************************************************
 * future_alloc.c - Initializes the future data structure.
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 10/06/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 12/10/2016
 * Assignment: 8,4
 ***************************************************************/

future* future_alloc(int future_flag)
{
  future *fut = NULL;
  
  if( future_flag != NULL) {
    fut = (future*)getmem(sizeof(future));
    fut->flag = future_flag;
    fut->state = FUTURE_EMPTY;
  }
  
  return fut;  
}
