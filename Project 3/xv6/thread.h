//CS202 - TODO
typedef struct lock_t lock_t;
typedef struct queue queue;
struct queue
{
	//int isEmpty;
	unsigned int pid;
	queue* next;
};
struct lock_t
{
	unsigned int flag;
	unsigned int guard;
	queue* q;	
};

void thread_create(void *(*start_routine)(void*), void* arg);
void lock_init(lock_t *lock);
void lock_acquire(lock_t *lock);
void lock_release(lock_t *lock);
//////////////////////////////////
void lock_qacquire(lock_t *lock);
void lock_qrelease(lock_t *lock);

void qput(queue** Q,unsigned int PID);
unsigned int qget(queue** Q);
void qprint(queue* Q);
int qlen(queue* Q);
