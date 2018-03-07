#include <iostream>
#include <stdlib.h>
#include <arm_neon.h>
#include <time.h>
#define MEM_BUFFER_SIZE 128*1024*1024
#define TIME_MASK 0x00003FFFFFFFF
using namespace std;

long long int period, sender, receiver, sender_loop, ignore_bits, numbits;
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
	return (time.tv_nsec + (time.tv_sec*1000000000));
}





int main(int argc, char* argv[])
{
	period = atoi(argv[1]);
	sender = atoi(argv[2]);
	receiver = atoi(argv[3]);
	sender_loop = atoi(argv[4]);
	ignore_bits = atoi(argv[5]);
	numbits = atoi(argv[6]);
	fillerData = (int*) malloc(sizeof(int) * MEM_BUFFER_SIZE);

//	cout << "Receiver Ready\n";
 //       string ready;
//        cin >> ready;

	long long int * probes = (long long int*) malloc(sizeof(long long int) * 2500);
        while((getTSC() & TIME_MASK) > 20000) {  }

        long long int startTime = getTSC();
//	cout << "Starttime = " << startTime << endl;	

        for(int i = 0; i < numbits + ignore_bits; i++){
                while(getTSC() < (startTime + (i * period))) { }
//		cout << "after "<< i << " period syncronization " << getTSC() << endl;
/* One load + mfence instruction pair takes ~200ns */
		long long probeStart = getTSC();
                for(int j = 0; j < receiver; j++)
                {
//~ count += data[j*j*j%15000000];
                                //~ __asm__ __volatile__( "mfence\n\t" );
                        unsigned int ind = i*i + j;
                        ind = (ind*ind);
                        ind = ind % MEM_BUFFER_SIZE;
                        ind &= 0xFFFFFFFB;
			int32x4_t i = {ind, ind, ind, ind};
                        vst1q_s32(&fillerData[ind], i);
                }
		
                long long int probeStop = getTSC();
		if ( i >= ignore_bits) {
                        int index = i - ignore_bits;
//                        printf("%lld\n", (index*2+1));
//			if(probes==NULL) printf("Null\n");
			probes[index*2+1] = probeStop - probeStart;
                        probes[index*2] = getTSC();
                }
        }

	long long int avg = 0;
        for (int i = 0; i < numbits; ++i) {
                avg += probes[i*2+1];
        }
        avg = avg/numbits;

        for (int i = 0; i < numbits; ++i) {
                int res = ((probes[i*2+1] > avg)) ? 0 : 1;
		if (i) 
		//	int diff = p
		cout << res << "\t" << probes[2*i+1] << "\t" << probes[2*i]-probes[2*i-2] << endl;
        }

}
