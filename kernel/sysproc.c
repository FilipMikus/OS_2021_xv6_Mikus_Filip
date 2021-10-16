#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// copy accessed pages to user-space
#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // sys_call arguments variables
  uint64 va_startpage;
  int numpages;
  uint64 bitmask_useraddr;
  
  uint64 bitmask_buf = 0;
  // max of scanned pages is 64 because uint64 is 64-bit
  int maxnumpages = 64;
  
  // get sys_call arguments
  if(argaddr(0, &va_startpage) < 0)
    return -1;
  if(argint(1, &numpages) < 0)
    return -1;
  if(argaddr(2, &bitmask_useraddr) < 0)
    return -1;
  
  // scan of pages 
  for(int i = 0; i < numpages && i < maxnumpages; i++){      // max limit of pages check
    pte_t *pte = walk(myproc()->pagetable, va_startpage, 0);
    
    // PTE validity check
    if(pte == 0)
      return -1;
    // check if pages was accessed
    if(*pte & PTE_A){
      bitmask_buf |= 1 << i;
      *pte &= ~PTE_A;
    }
    va_startpage += PGSIZE;
  }
  // copy bitmask to user-space
  if(copyout(myproc()->pagetable, bitmask_useraddr, (char *)&bitmask_buf, (numpages+7)/8) < 0)
    return -1;
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
