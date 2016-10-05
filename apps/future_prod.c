#include <xinu.h>
#include <future.h>

uint future_prod(future *fut) {
  //printf("\nInside future_prod");
  int i, j;
  j = (int)fut;
  for (i=0; i<1000; i++) {
    j += i;
  }
  future_set(fut, &j);
  printf("\n[Process: %d] Produced: %d", getpid(), j);
  return OK;
}
