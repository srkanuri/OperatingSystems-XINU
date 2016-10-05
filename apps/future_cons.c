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
  printf("\n[Process: %d] Consumed: %d", getpid(),  i);
  return OK;
}
