#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "--- Initial Page Table ---\n");
  vmprint();

  printf(1, "\n--- Allocating 4 Pages (4 * 4096 bytes) ---\n");
  char *p = sbrk(4 * 4096);
  
  // Touch memory so pages are allocated in lazy system
  for(int i = 0; i < 4; i++) {
    p[i * 4096] = 1;
  }

  printf(1, "--- Page Table After Allocation ---\n");
  vmprint();

  exit();
}