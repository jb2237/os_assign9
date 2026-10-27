// ... inside trap() switch statement ...
  case T_PGFLT: {
    uint va = rcr2(); // Get faulting address
    struct proc *curproc = myproc();

    // Check if the faulting address is valid and below myproc()->sz
    if (va < curproc->sz && va >= 0) {
      char *mem = kalloc();
      if (mem == 0) {
        cprintf("out of memory\n");
        curproc->killed = 1;
        break;
      }
      memset(mem, 0, PGSIZE);
      if (mappages(curproc->pgdir, (char*)PGROUNDDOWN(va), PGSIZE, V2P(mem), PTE_W | PTE_U) < 0) {
        cprintf("out of memory (mappages)\n");
        kfree(mem);
        curproc->killed = 1;
        break;
      }
      curproc->page_faults++; // Increment page fault counter
    } else {
      cprintf("pid %d %s: trap %d err %d on cpu %d eip 0x%x addr 0x%x--kill proc\n",
              curproc->pid, curproc->name, tf->trapno, tf->err, cpunum(), tf->eip, va);
      curproc->killed = 1;
    }
    break;
  }