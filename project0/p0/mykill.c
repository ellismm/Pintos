#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define SUCCESS 0
#define FAILURE 1
#define KILL_FAILURE 2

// J. RANKIN drove here
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "%s: Not enough arguments\n", argv[0]);
        return FAILURE;
    }
    char *endptr = NULL;
    errno = 0;
    long first_arg = strtol(argv[1], &endptr, 10);
    if (errno != 0 || endptr == argv[1]) {
        // not a number
        fprintf(stderr, "%s: argument 1 should be a number\n", argv[0]);
        return FAILURE;
    }

    pid_t tgt_pid = (pid_t) first_arg;

    int kill_res = kill(tgt_pid, SIGUSR1);
    if (kill_res != 0) {
        perror(argv[0]);
        return KILL_FAILURE;
    }

    return SUCCESS;
}
