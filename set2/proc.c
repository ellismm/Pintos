#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/param.h>

typedef long long hrtime_t;
int returnOne(void);

void main(char *argc, char **argv) {
   struct timeval  start, end, sub;

   gettimeofday(&start, NULL);
   uid_t call = getuid();
   gettimeofday(&end, NULL);
   timersub(&end, &start, &sub);

   sub.tv_usec *= 1000;

   printf("time in microseconds a of system call %ld\n", sub.tv_usec);
   
   gettimeofday(&start, NULL);
   int proc = returnOne(); 
   gettimeofday(&end, NULL);
   timersub(&end, &start, &sub);
   sub.tv_usec *= 1000;

   printf("time in microseconds a of procedure call %ld\n", sub.tv_usec);
   
//   printf("time in nanoseconds: %lld\n", end - start);
}

int returnOne(void) {
    return 1;
}

