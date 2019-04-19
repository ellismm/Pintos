#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "util.h"

void sigint_handler(int);
void sigusr1_handler(int);


/*
 * First, print out the process ID of this process.
 *
 * Then, set up the signal handler so that ^C causes
 * the program to print "Nice try.\n" and continue looping.
 *
 * Finally, loop forever, printing "Still here\n" once every
 * second.
 */
int main(int argc, char **argv)
{
    // M. ELLIS drove here
    Signal(SIGINT, sigint_handler);
    Signal(SIGUSR1, sigusr1_handler);
    // J. RANKIN and M. ELLIS drove here
    pid_t my_pid = getpid();
    printf("%d\n", my_pid);

    struct timespec one_second;
    one_second.tv_sec = 1;
    one_second.tv_nsec = 0;
    struct timespec rem;

    while (1) {
        puts("Still here");
        int sleep_outcome = nanosleep(&one_second, &rem);
        struct timespec next_rem;
        // J. RANKIN drove here
        while (sleep_outcome == -1) {
            sleep_outcome = nanosleep(&rem, &next_rem);
            rem.tv_sec = next_rem.tv_sec;
            rem.tv_nsec = next_rem.tv_sec;
        }
    }

    return 0;
}

void sigint_handler(int sig) {
    // J. RANKIN and M. ELLIS drove here
    // Signal handler safe I/O from project description
    ssize_t STR_LEN = 10;
    int FD_STDOUT = 1;
    ssize_t bytes = write(FD_STDOUT, "Nice try.\n", STR_LEN);
    if(bytes != STR_LEN) {
        exit(-999);
    }
}

void sigusr1_handler(int sig) {
    // M. ELLIS drove here
    ssize_t STR_LEN = 8;
    int FD_STDOUT = 1;
    ssize_t bytes = write(FD_STDOUT, "exiting\n", STR_LEN);
    if(bytes != STR_LEN) {
        exit(-999);
    }
    exit(1);
}

