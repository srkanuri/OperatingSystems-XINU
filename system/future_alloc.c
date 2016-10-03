/* future_alloc.c - future_alloc */

#include <xinu.h>
#include <future.h>

/*------------------------------------------------------------------------
 *  future_alloc- Initializes the future data structure.
 *------------------------------------------------------------------------
 */
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
