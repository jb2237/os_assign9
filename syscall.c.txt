extern int sys_vmprint(void);
extern int sys_getfaults(void);

static int (*syscalls[])(void) = {
  // ...
  [SYS_vmprint]   sys_vmprint,
  [SYS_getfaults] sys_getfaults,
};