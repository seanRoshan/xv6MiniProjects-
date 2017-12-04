//TODO
#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread.h"

//int counter=0;

struct argThread
{
	int* iterNum; // Shared
	int* counter; // Shared	
	int* token;   // Shared
	int* totalT;  // Shared
	int* runtime; // Shared
	lock_t *lock; // Shared
	int  id;      // Local
	
};
typedef struct argThread argThread;

void* ball(void* arg)
{
	argThread* n=(argThread*) arg;
	int pid=getpid();
	while(1)
	{
		lock_qacquire(n->lock);	
		(*n->runtime)++;	
		if(*n->counter>=*n->iterNum) 
		{
			lock_qrelease(n->lock);
			break;
		}

		if (*n->token == n->id){ 
			printf(1, "TOKEN: %d - THREAD# %d:\n%d This message can be cut into by another thread unless we have locks installed.\nIf the numbers in the beginning and the end are equal, the message is whole! %d\n*****\n",*n->token,pid,*n->counter,*n->counter);
			*n->token = (*n->token+1)%(*n->totalT);
			(*n->counter)++;
		}
		
		lock_qrelease(n->lock);
	}
	
	return 0;	
}

int
main(int argc, char *argv[])
{
  //Argument initialization
  int maxLimit=10,numThread=2,counter=0,frisbee=0,runtime=0,i;
  lock_t lock;
  if(argc>1) numThread=atoi(argv[1]);
  if(argc>2) maxLimit=atoi(argv[2]);
  lock_init(&lock);
  
  //Argument bundle for threads
  argThread *arg_t = (argThread*)malloc(numThread*sizeof(argThread));
  for (i=0; i<numThread; i++)
  {
	  arg_t[i].counter=&counter;
	  arg_t[i].iterNum=&maxLimit;
      arg_t[i].lock=&lock;
      arg_t[i].runtime=&runtime;
      arg_t[i].totalT=&numThread;
      arg_t[i].token = &frisbee;
      arg_t[i].id=i;
      
  }
  //Begin weaving the threads!
  //void* t=(void*)&arg_t;
  for(i=0;i<numThread;i++) 
  {
	  thread_create(ball,(void*)&(arg_t[i])); 
  }
  for(i=0;i<numThread;i++) wait();	//1 wait for every thread

  //End
  printf(1, "Your time has come! Final Counter Value = %d Runtime = %d\n",counter,runtime);
  free(arg_t);
  exit();
}
