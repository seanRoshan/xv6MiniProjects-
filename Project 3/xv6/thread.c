//CS202 - TODO 
#include "types.h"
#include "user.h"
#include "thread.h"
#include "x86.h"	//For xchg(), the atomic set function
#include <stddef.h>	//For NULL

void thread_create(void *(*start_routine)(void*), void* arg)
{
	int size=4096*sizeof(void);
	void* stack= (void*)malloc(size);
	
	int childID;
	childID=clone(stack,size);
	
	if(childID==0)
	{
		//printf(1, "Thread %d Created!!\n",getpid());
		(*start_routine)(arg);
		exit();
	}
}

void lock_init(lock_t *lock)
{
	lock->flag=0;
	lock->guard=0;
	lock->q=NULL;
	
}

void lock_acquire(lock_t *lock)		//Spinlock
{
	while(xchg(&lock->flag,1)==1);	
}

void lock_release(lock_t *lock)
{
	xchg(&lock->flag,0);
}
///////////////////////////////////////////////////////////////
// If you read the three easy pieces book, you can find the similar code :) 
// Acuring the queue lock 
void lock_qacquire(lock_t *lock)		//Mr. Anderson's lock!
{
	while(xchg(&lock->guard,1)==1);	
	if(lock->flag==0)
	{
		lock->flag=1;
		lock->guard=0;
	}
	else
	{
		qput(&lock->q,getpid());
		lock->guard=0;
		thsleep();
	}	
}
// Releasing the queue lock
void lock_qrelease(lock_t *lock)
{
	while(xchg(&lock->guard,1)==1);	
	if(qlen(lock->q)==0)
	{
		lock->flag=0;
	}
	else
	{
		unsigned PID=qget(&lock->q);
		if(PID!=0) thwake(PID);
	}
	lock->guard=0;
	
}

// Pushing the element at the end of the queue
void qput(queue** Q,unsigned int PID)
{
	queue* qtemp;
	int c=0;
	
	qtemp=(queue*)malloc(sizeof(queue));
	qtemp->pid=PID;
	qtemp->next=NULL;
	
	if(*Q==NULL) 
	{
		*Q=qtemp;
//		printf(1,"QUEUE ANNOUNCEMENT: QUEUE CREATED!\n");
	}
	else
	{
		queue* qtemp2=*Q;
		while(qtemp2->next!=NULL) {qtemp2=qtemp2->next;c++;}
		qtemp2->next=qtemp;c++;
	}
	
//	printf(1,"QUEUE ANNOUNCEMENT: %d has been put in location %d.\n",qtemp->pid,c);
}

// Get the element at the front of the queue
unsigned int qget(queue** Q)
{
	if(*Q!=NULL)
	{
		unsigned int targetPID=(*Q)->pid;
		if((*Q)->next!=NULL)
		{
					queue* qtemp=*Q;
					*Q=(*Q)->next;
					free(qtemp);
		}
		else
		{
			free(*Q);
			*Q=NULL;
//			printf(1,"QUEUE ANNOUNCEMENT: QUEUE EMPTY!\n");
		}	
		
//		printf(1,"QUEUE ANNOUNCEMENT: %d has been removed.\n",targetPID);
		return targetPID;
	}
	return 0;
}
// Prints the content of the queue
void qprint(queue* Q)
{
	queue* qtemp=Q;
	printf(1,"QUEUE CONTENT: ");
	while(qtemp!=NULL)
	{
		printf(1,"\t%d",qtemp->pid);
		qtemp=qtemp->next;
	}
	printf(1,"\n");
}
// Determines the length of the queue
int qlen(queue* Q)
{
	queue* qtemp=Q;
	int len=0;
	while(qtemp!=NULL)
	{
		len++;
		qtemp=qtemp->next;
	}
	return len;
}
