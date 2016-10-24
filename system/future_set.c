/* future_set.c - future_set */

#include <xinu.h>
#include <future.h>
/***************************************************************
 * future_set.c - Sets the value of future and sets the status to FUTURE_VALID
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 10/06/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 10/06/2016
 * Assignment: 4
 ***************************************************************/

syscall future_set(future* f, int* value)
{
  if(f == NULL || value == NULL || f->state == FUTURE_VALID) {
    return SYSERR;
  }

  f->value = (int *)getmem(sizeof(int));

  // Null check.
  if(f->value == NULL) {
  	return SYSERR;
  }

  f->value = *value;
  //  printf("Prod PID %d\n",getpid());
  //  printf("Prod PID Value %d\n",*value);
  if(f->flag == FUTURE_EXCLUSIVE){
    if(f->state == FUTURE_WAITING) {
      //printf("[Process: %d] Resuming process %d\n", getpid(), f->pid);
      f->state = FUTURE_VALID;
      resume(f->pid);
    }
  }
  if(f->flag == FUTURE_SHARED){
      //printf("Main PID %d",f->get_queue);
    f->state = FUTURE_VALID;  
    while(f->get_queue != NULL){
      pid32 cons_pid = dequeue_process(&f->get_queue);
      resume(cons_pid);
      }
    future_free(f);
  }
  else{
    if(f->get_queue == NULL){
      pid32 curr_pid = getpid();
      enqueue_process(&f->set_queue, curr_pid);
      //printf("[Process: %d] Set to Suspend\n",  curr_pid);
      suspend(curr_pid);
    }
    else{
      //f->state = FUTURE_VALID;
      pid32 cons_pid = dequeue_process(&f->get_queue);
      //printf("%d\n", cons_pid);
      resume(cons_pid);
    }
  }
  return OK;
}

