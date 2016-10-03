/* future_set.c - future_set */

#include <xinu.h>
#include <future.h>

/*-----------------------------------------------------------------------------
 *  future_set- Sets the value of future and sets the status to FUTURE_VALID
 *-----------------------------------------------------------------------------
 */
syscall future_set(future* f, int *value)
{

  if(f == NULL || value == NULL) {
    return SYSERR;
  }

  f->value = *value;
  f->state = FUTURE_VALID;

  return OK;
}

