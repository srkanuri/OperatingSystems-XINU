#include <xinu.h>

/***************************************************************
 * future_queue.h - Header file for Future get and set queues.
 ***************************************************************/

// Max number of processes is 100.
#define MAX_QUEUE_LEN = 100

// Wait queue represented as single linked list. Each node holds pid.
typedef struct queue_node
{
  pid32 pid;
  struct queue_node *next;
} queue;

/* Interfaces for set and get queue system calls */
uint enqueue_process(queue*, pid32);
pid32 dequeue_process(queue*);