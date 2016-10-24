/* future_get.c - future_get */

#include <xinu.h>
#include <future.h>
/***************************************************************
 * future_get.c - Gets the value of future and sets the status to FUTURE_EMPTY
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 10/06/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 10/06/2016
 * Assignment: 4
 ***************************************************************/

syscall future_get(future* f, int* value)
{
  
  if(f == NULL || value == NULL || f->state == FUTURE_WAITING) {
    return SYSERR;
  }
  pid32 curr_pid = getpid();
  if(f->flag == FUTURE_EXCLUSIVE){
    if( f->state == FUTURE_EMPTY ) {
      f->pid = curr_pid;
      f->state = FUTURE_WAITING;
      printf("[Process: %d] Set to Suspend\n",  f->pid);
      suspend(f->pid);
    }
    value = *f->value;
    f->state = FUTURE_EMPTY;
  }
  else{
      //printf("%d\n",f->get_queue);
      enqueue_process(&f->get_queue, curr_pid);
      //printf("%d\n",f->get_queue);
      printf("[Process: %d] Sent to Suspend\n",  curr_pid);
      suspend(curr_pid);
  }
  return OK;
}

