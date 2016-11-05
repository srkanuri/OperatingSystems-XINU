/* enqueue_process.c - enqueue_process */

#include <xinu.h>
#include <future.h>
/*************************************************************************
 * enqueue_process.c - Enqueues process into the future set or get queue.
 *************************************************************************/

uint enqueue_process(fqueue** procq, pid32 procid) {

  fqueue* qtmp = *procq;
  fqueue* new_node = (fqueue*)getmem(sizeof(fqueue));

  // Null check
  if(new_node == NULL) {
    return SYSERR;
  }

  // Append the new process node to the queue.
  new_node->pid = procid;
  new_node->next = NULL;
  if(*procq == NULL){
       //printf("Old %d\n", *procq);
	*procq = new_node;
       //printf("New %d\n", *procq);
  }
  else{
       for(; qtmp->next != NULL; qtmp = qtmp->next);
       qtmp->next = new_node;
  }

  return OK;

}
