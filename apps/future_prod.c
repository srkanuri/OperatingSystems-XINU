#include <xinu.h>
#include <future.h>

uint future_prod(future *fut) {
  int i, j;
  j = (int)fut;
  for (i=0; i<1000; i++) {
    j += i;
  }
  printf("[Process: %d] Produced: %d\n", getpid(), j);
  future_set(fut, &j);
  return OK;
}
