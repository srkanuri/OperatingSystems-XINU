/* enqueue_process.c - enqueue_process */

#include <xinu.h>
#include <future.h>
/*************************************************************************
 * enqueue_process.c - Enqueues process into the future set or get queue.
 *************************************************************************/

uint enqueue_process(queue* procq, pid32 procid) {

  queue* new_node = (queue*)getmem(sizeof(queue));

  // Null check
  if(new_node == NULL) {
    return SYSERR;
  }

  // Append the new process node to the queue.
  new_node->pid = procid;
  new_node->next = procq;
  procq = new_node;

  return OK;

}
