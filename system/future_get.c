/* future_get.c - future_get */

#include <xinu.h>
#include <future.h>

/*---------------------------------------------------------------------------
 *  future_get- Gets the value of future and sets the status to FUTURE_EMPTY
 *---------------------------------------------------------------------------
 */
syscall future_get(future* f, int* value)
{
  
  if(f == NULL || value == NULL || f->state == FUTURE_WAITING) {
    return SYSERR;
  }
  
  if( f->state == FUTURE_EMPTY ) {
    f->pid = getpid();
    f->state = FUTURE_WAITING;
    printf("\n[Process: %d] Suspending process %d", f->pid, f->pid);
    suspend(f->pid);    
  }
  value = *f->value;
  //printf("\n[Process: %d] Future value read: %d", getpid(), value);
  f->state = FUTURE_EMPTY;
  return OK;
}

