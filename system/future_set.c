/* future_set.c - future_set */

#include <xinu.h>
#include <future.h>

/*-----------------------------------------------------------------------------
 *  future_set- Sets the value of future and sets the status to FUTURE_VALID
 *-----------------------------------------------------------------------------
 */
syscall future_set(future* f, int* value)
{
  //printf("\n[Process: %d] Inside future_set", getpid());

  if(f == NULL || value == NULL || f->state == FUTURE_VALID) {
    return SYSERR;
  }

  f->value = (int *)getmem(sizeof(int));
  f->value = *value;
  //printf("\n[Process: %d] Future Value set: %d", getpid(), f->value);
  
  if(f->state == FUTURE_WAITING) {
    printf("[Process: %d] Resuming process %d\n", getpid(), f->pid);
    resume(f->pid);
  }
  f->state = FUTURE_VALID;

  return OK;
}

