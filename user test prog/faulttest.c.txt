#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int sz = 32 * 4096;
  char *buf = sbrk(sz);

  printf(1, "Allocated 32 pages lazily.\n");

  // Accessing exactly 5 distinct pages
  buf[0 * 4096] = 'a';
  buf[2 * 4096] = 'b';
  buf[5 * 4096] = 'c';
  buf[10 * 4096] = 'd';
  buf[20 * 4096] = 'e';

  int faults = getfaults();
  printf(1, "Page faults handled: %d (Expected: 5)\n", faults);

  // Testing invalid access above allocated size
  printf(1, "Testing illegal access above allocated size...\n");
  char invalid = buf[33 * 4096]; // Should cause process termination
  (void)invalid;

  printf(1, "ERROR: Should not reach here!\n");
  exit();
}