static struct proc*
allocproc(void)
{
  // ... existing allocproc code ...
  
  p->pid = nextpid++;
  p->state = EMBRYO;
  p->page_faults = 0; // Initialize page fault counter

  return p;
}