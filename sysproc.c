#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

// Question 1: System call to print page table
int
sys_vmprint(void)
{
  return vmprint();
}

// Question 2: Modified sbrk to implement lazy allocation
int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  struct proc *curproc = myproc();
  addr = curproc->sz;
  
  if (n > 0) {
    // Lazily grow memory size without allocating physical pages immediately
    curproc->sz += n;
  } else if (n < 0) {
    // Shrink memory immediately
    curproc->sz = deallocuvm(curproc->pgdir, curproc->sz, curproc->sz + n);
  }
  
  return addr;
}

// Question 2: System call to return lazy allocation page fault count
int
sys_getfaults(void)
{
  return myproc()->page_faults;
}