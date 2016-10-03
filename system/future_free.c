/* future_free.c - future_free */

#include <xinu.h>
#include <future.h>

/*------------------------------------------------------------------------
 *  future_free- Frees of the memory allocated for future.
 *------------------------------------------------------------------------
 */
syscall future_free(future* f)
{
  return freemem(f, sizeof(f));
}

