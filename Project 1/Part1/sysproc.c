#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"



int
sys_fork(void)
{
  // DRSVR - CS202 - LAB0 PART 1 BEGIN
  // FORK WILL START A PROCESS, RESET NUMBER OF SYSCALLS TO 0
	proc->sysCount = 0;
  // DRSVR - CS202 - LAB0 PART 1 END
  return fork();
}

int
sys_exit(void)
{
  //sys_DRSVR_returncount();
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// DRSVR - CS202 - LAB0 PART 1
// returns a count of the total number of system calls 
// that a process has done so far
int
sys_returncount(void)
{
	cprintf("------------------------------------------------------------------------\n");	
	
	cprintf("Attention:\nnumber of system calls for each process has been returned!\n");
	
	cprintf("%d %s: sys call count: %d\n",
            proc->pid, proc->name, proc->sysCount); 
    
    cprintf("------------------------------------------------------------------------\n");
	
	return 0;
}



