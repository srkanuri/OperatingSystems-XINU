#include <xinu.h>
#include <future.h>

uint future_cons(future *fut) {
  //printf("\n[Process: %d] Inside future_cons", getpid());
  int i, status;
  status = future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("[Process: %d] Consumed: %d\n", getpid(),  fut->value);
  return OK;
}
