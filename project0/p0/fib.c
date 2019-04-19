#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

const int MAX = 13;
const int ERR = -1;

static void doFib(int n, int doPrint);


/*
 * unix_error - unix-style error routine.
 */
inline static void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}


int main(int argc, char **argv)
{
    int arg;
    int print=1;

    if(argc != 2){
        fprintf(stderr, "Usage: fib <num>\n");
        exit(-1);
    }

    arg = atoi(argv[1]);
    if(arg < 0 || arg > MAX){
        fprintf(stderr, "number must be between 0 and %d\n", MAX);
        exit(-1);
    }

    doFib(arg, print);

    return 0;
}

/* 
 * Recursively compute the specified number. If print is
 * true, print it. Otherwise, provide it to my parent process.
 *
 * NOTE: The solution must be recursive and it must fork
 * a new child for each call. Each process should call
 * doFib() exactly once.
 */
static void doFib(int n, int doPrint)
{
    // J. RANKIN and M. ELLIS both drove here intermittently
    int result = 0;
    pid_t fork_pid;
    switch (n) {
        case 0:
            result = 0;
            break;
        case 1:
            result = 1;
            break;
        default:
            fork_pid = fork();
            if (fork_pid == ERR) {
                unix_error("fork()");
            } else if (fork_pid == 0) {
                // the child process, 1st (n-1) invocation
                doFib(n - 1, 0);
            } else {
                // the parent process, 1st (n-1) invocation
                int child_status = 0;
                pid_t wait_res = wait(&child_status);
                if (wait_res == ERR)
                    unix_error("wait()");
                result = WEXITSTATUS(child_status);

                // second recursive invocation (n-2)
                pid_t nminus2_pid = fork();
                if (nminus2_pid == ERR) {
                    unix_error("fork()");
                } else if (nminus2_pid == 0) {
                    // the child process, 2nd (n-2) invocation
                    doFib(n - 2, 0);
                } else {
                    // the parent process, 2nd (n-2) invocation
                    int nm2_status = 0;
                    pid_t wait2_res = wait(&nm2_status);
                    if (wait2_res == ERR)
                        unix_error("wait()");
                    result += WEXITSTATUS(nm2_status);
                }
            }
    }

    if (doPrint) {
        printf("%d\n", result);
    } else {
        exit(result);
    }
}

