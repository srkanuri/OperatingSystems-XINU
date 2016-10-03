/* future_get.c - future_get */

#include <xinu.h>
#include <future.h>

/*---------------------------------------------------------------------------
 *  future_get- Gets the value of future and sets the status to FUTURE_EMPTY
 *---------------------------------------------------------------------------
 */
syscall future_get(future* f, int *value)
{
  
  if(f == NULL || value == NULL) {
    return SYSERR;
  }
  
  *value = f->value;
  f->state = FUTURE_EMPTY;

  return OK;
}

