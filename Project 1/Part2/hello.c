#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"

/* NOTE FOR RANDOM FUNCTION GENERATOR
 * http://linux.die.net/man/3/rand
 * POSIX.1-2001 gives the following example of an implementation 
 * of rand() and srand(), possibly useful when one needs the same 
 * sequence on two different machines.
*/

static unsigned long next = 1;

/* RAND_MAX assumed to be 32767 */
int myrand(void) {
    next = next * 1103515245 + 12345;
    return((unsigned)(next/65536) % 32768);
}

void mysrand(unsigned seed) {
    next = seed;
}

// DRSVR - CS202 - LAB0 PART 2
// GENERATE RANDOM NUMBER :) 
int generateRandom (unsigned max) {
	
	int myRandom = 1;
	mysrand(max);
	myRandom = (myrand() % max);
	if (myRandom>0) {
		return (myRandom);
	}
	else {
		return (1);
	}
}
   
   
int main(int argc, char *argv[])
{
  
    
	printf(1, "DRSVR TEST 0 HAS BEEN STARTED!\n");
	   
    buyticket(generateRandom(atoi(argv[1])));
    
    returncount();
	
	exit();
}
