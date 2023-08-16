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


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 start_va;
  if(argaddr(0, &start_va) < 0)
    return -1;

  int page_num;
  if(argint(1, &page_num) < 0)
    return -1;

  if (page_num > 64)
  {
    panic("pgaccess: too much pages");
    //使用panic，引出异常
    return -1;
  }

  uint64 result_va;
  if(argaddr(2, &result_va) < 0)
    return -1;

  unsigned int bitmask = 0;
  int cur_bitmask = 1;
  int count = 0;
  pte_t *pte;
  struct proc *p = myproc();

  for (; count < page_num; count++)
  {
    if ((pte = walk(p->pagetable, start_va, 0)) == 0)
      panic("pgaccess: pte should exist");
    if ((*pte & PTE_A))
    {
      bitmask = bitmask | (cur_bitmask<<count);
      *pte = *pte & ~PTE_A;
      //清除PTE_A位
    }
    start_va += PGSIZE;
  }
  copyout(p->pagetable,result_va,(char*)&bitmask,sizeof(bitmask));

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
