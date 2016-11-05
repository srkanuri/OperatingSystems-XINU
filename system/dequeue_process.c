/* dequeue_process.c - dequeue_process */

#include <xinu.h>
#include <future.h>

/*************************************************************************
 * dequeue_process.c - Dequeues process from the future set or get queue.
 * Returns the dequeued process node or NULL if the queue is empty.
 *************************************************************************/

pid32 dequeue_process(fqueue** procq) {

  fqueue* front_node = *procq;

  *procq = (*procq)->next;

  pid32 procid = front_node->pid;

  uint status = freemem((char*)front_node, sizeof(fqueue));

  if(status == SYSERR) {
    return (pid32)SYSERR;
  }

  return procid;
}
