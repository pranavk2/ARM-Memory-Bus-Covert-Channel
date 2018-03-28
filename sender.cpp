#include <iostream>
#include <stdlib.h>
#include <smmintrin.h>
#include <time.h>

#define TIME_MASK 0x000003FFFFFFFF
#define MEM_BUFFER_SIZE 128*1024*1024

using namespace std;

int period, sender, receiver, sender_loop, ignore_bits, stream_length;
int * fillerData;

static inline long long int getTSCold2() {

        long long int tsc;
        __asm__ volatile("rdtsc; " // read of tsc
			 "shl $32,%%rdx; "  // shift higher 32 bits stored in rdx up
                         "or %%rdx,%%rax"   // and or onto rax
			 : "=a"(tsc)        // output to tsc
                         :
                         : "%rcx", "%rdx", "memory");

        return tsc;
}


long long int getTSC() {
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return (time.tv_nsec+(time.tv_sec*1000000000));
}

void send0(long long stop, int sender_loop)
{
	unsigned long ind;
	unsigned int seed = rand();
	do {
		for(int n=0; n < sender_loop; n++) {
			ind = seed + n;
			ind *= ind;
			__asm__ volatile ( "lock xchg %%rax, (%0) \n\t"
                         :
                         : "r"(fillerData+(ind % (MEM_BUFFER_SIZE-3)))
                         : "%rax");
		}

	} while(getTSC() < stop);
}



int main(int argc, char* argv[]) 
{
	period = atoi(argv[1]);
	sender = atoi(argv[2]);
	receiver = atoi(argv[3]);
	sender_loop = atoi(argv[4]);
	ignore_bits = atoi(argv[5]);
	stream_length = atoi(argv[6]);
	fillerData = (int*) malloc (sizeof(int) * MEM_BUFFER_SIZE);
	//srand(time(NULL));
//	cout << "Sender Ready\n";
	//string ready;
	//cin >> ready;
	while((getTSC() & TIME_MASK) > 20000) {  }
//	cout << getTSC() << endl;
	long long int startTime = getTSC();

        for(int n=0; n < stream_length + ignore_bits; n++){

                while(getTSC() < startTime + (n * period)) { }

                long long int stopTime = startTime + sender + (n * period);
                if (n < ignore_bits){
                        if (1)  // always send 0's during the initial ignore bits period
                                send0(stopTime, sender_loop);
                } else if(n % 3) // after the inital ignore bits phase, send alternate 0's and 1's
                        send0(stopTime, sender_loop);
        }
}
