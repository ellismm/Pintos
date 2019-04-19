/*
 * msh - A mini shell program with job control
 * 
 * JARED RANKIN, jrankin
 * MARK ELLIS, mark
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "util.h"
#include "jobs.h"

#define ERR (-1)


/* Global variables */
int verbose = 0;            /* if true, print additional output */

extern char **environ;             /* defined in libc */
static char prompt[] = "msh> ";    /* command line prompt (DO NOT CHANGE) */
static struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
void usage(void);
void sigquit_handler(int sig);
void signal_safe_write(const char *, size_t);


/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) {
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */
    
    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);
    
    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
            case 'h':             /* print help message */
                usage();
                break;
            case 'v':             /* emit additional diagnostic info */
                verbose = 1;
                break;
            case 'p':             /* don't print a prompt */
                emit_prompt = 0;  /* handy for automatic testing */
                break;
            default:
                usage();
        }
    }
    
    /* Install the signal handlers */
    
    /* These are the ones you will need to implement */
    Signal(SIGINT, sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */
    
    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler);
    
    /* Initialize the job list */
    initjobs(jobs);
    
    /* Execute the shell's read/eval loop */
    while (1) {
        
        /* Read command line */
        if (emit_prompt) {
            printf("%s", prompt);
            fflush(stdout);
        }
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
            app_error("fgets error");
        if (feof(stdin)) { /* End of file (ctrl-d) */
            fflush(stdout);
            exit(0);
        }
        
        /* Evaluate the command line */
        eval(cmdline);
        fflush(stdout);
        fflush(stdout);
    }
    
    exit(0); /* control never reaches here */
}


/*
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
 */
void eval(char *cmdline) {
    char *argv[MAXARGS];
    int is_bg = parseline(cmdline, argv);
    // J. RANKIN drove here
    if (!builtin_cmd(argv)) {
        // command not built in, run from the filesystem
        
        // block SIGCHLD signals until we finish generating the job
        sigset_t sigchld_only, old_set;
        sigemptyset(&sigchld_only);
        sigaddset(&sigchld_only, SIGCHLD);
        sigprocmask(SIG_BLOCK, &sigchld_only, &old_set);
        
        // create associated job and process
        pid_t fork_pid = fork();
        if (fork_pid == ERR) {
            perror("fork()");
            return;
        } else if (fork_pid == 0) {
            // child process (the one running the command)
            setpgid(0, 0);
            sigprocmask(SIG_UNBLOCK, &sigchld_only, &old_set);
            execve(argv[0], argv, environ);
            
            if (errno == ENOENT) {
                fprintf(stdout, "%s: Command not found\n", argv[0]);
                exit(127);
            } else {
                unix_error(argv[0]);
            }
        }
        // parent process (the one running the shell)
        // track as job
        int job_status = is_bg ? BG : FG;
        addjob(jobs, fork_pid, job_status, cmdline);
        struct job_t *new_job = getjobpid(jobs, fork_pid);
        
        sigprocmask(SIG_UNBLOCK, &sigchld_only, &old_set);
        if (is_bg) {
            printf("[%d] (%d) %s", new_job->jid, fork_pid, cmdline);
        } else {
            waitfg(fork_pid);
        }
    }
}

/*
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 * Return 1 if a builtin command was executed; return 0
 * if the argument passed in is *not* a builtin command.
 */
int builtin_cmd(char **argv) {
    // J. RANKIN drove here
    if (argv == NULL || argv[0] == NULL) {
        return 1;
    }
    if (strcmp(argv[0], "quit") == 0) {
        exit(0);
    } else if (strcmp(argv[0], "jobs") == 0) {
        listjobs(jobs);
        return 1;
    } else if (strcmp(argv[0], "fg") == 0 || strcmp(argv[0], "bg") == 0) {
        do_bgfg(argv);
        return 1;
    }
    return 0;     /* not a builtin command */
}

/*
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) {
    // J. RANKIN and M. ELLIS drove here intermittently
    if (argv[1] == NULL) {
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }
    struct job_t *tgt_job = NULL;
    char *job_id_s = argv[1];
    
    if (*job_id_s == '%') {
        // job identified by %jobid
        errno = 0;
        // job_id_s + 1: everything after %
        int job_id = (int) strtol(job_id_s + 1, NULL, 10);
        // lookup job (strtol err -> attempted ref to invalid job 0)
        tgt_job = getjobjid(jobs, job_id);
        if (tgt_job == NULL) {
            printf("%s: No such job\n", job_id_s);
            return;
        }
    } else {
        // otherwise, job identified by process ID
        char *endptr = NULL;
        errno = 0;
        pid_t proc_id = (pid_t) strtol(job_id_s, &endptr, 10);
        if (errno != 0 || endptr == job_id_s) {
            printf("%s: argument must be a PID or %%jobid\n", argv[0]);
            return;
        }
        // lookup job
        tgt_job = getjobpid(jobs, proc_id);
        if (tgt_job == NULL) {
            printf("(%d): No such process\n", proc_id);
            return;
        }
    }
    
    // J. RANKIN drove here
    if (strcmp(argv[0], "fg") == 0) {
        // resume the job in the foreground
        tgt_job->state = FG;
        kill(-(tgt_job->pid), SIGCONT);
        waitfg(tgt_job->pid);
    } else if (strcmp(argv[0], "bg") == 0) {
        // resume the job in the background
        // (and put out a message indicating as such)
        tgt_job->state = BG;
        printf("[%d] (%d) %s", tgt_job->jid, tgt_job->pid, tgt_job->cmdline);
        kill(-(tgt_job->pid), SIGCONT);
    }
}

/*
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
    // J. RANKIN drove here
    sigset_t allowed_sigs, sigchld_only, old_sigset;
    
    sigemptyset(&sigchld_only);
    sigaddset(&sigchld_only, SIGCHLD);
    
    sigfillset(&allowed_sigs);
    
    sigdelset(&allowed_sigs, SIGINT);
    sigdelset(&allowed_sigs, SIGCHLD);
    sigdelset(&allowed_sigs, SIGTSTP);
    
    sigprocmask(SIG_BLOCK, &sigchld_only, &old_sigset);
    while (fgpid(jobs) == pid) {
        sigprocmask(SIG_UNBLOCK, &sigchld_only, &old_sigset);
        sigsuspend(&allowed_sigs);
        sigprocmask(SIG_BLOCK, &sigchld_only, &old_sigset);
    }
    sigprocmask(SIG_UNBLOCK, &sigchld_only, &old_sigset);
}

/*****************
 * Signal handlers
 *****************/

/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) {
    // J. RANKIN and M. ELLIS drove here intermittently
    char death_message[512];
    const char *termed = "Job [%d] (%d) terminated by signal %d\n";
    const char *stopped = "Job [%d] (%d) stopped by signal %d\n";
    
    for (int i = 0; i < MAXJOBS; i++) {
        struct job_t *job = &jobs[i];
        
        if (job->pid != 0) {
            int status = 0;
            pid_t waited_pid = waitpid(job->pid, &status, WNOHANG | WUNTRACED);
            
            if (waited_pid == ERR) {
                snprintf(death_message, 511,
                         "error while waiting for job %d: %s\n",
                         job->jid, strerror(errno));
                signal_safe_write(death_message, strlen(death_message));
            } else if (waited_pid != 0) {
                if (WIFSTOPPED(status)) {
                    // stop: stop the job, write the signal out
                    job->state = ST;
                    snprintf(death_message, 511, stopped,
                             job->jid, job->pid, WSTOPSIG(status));
                    signal_safe_write(death_message, strlen(death_message));
                } else if (WIFSIGNALED(status)) {
                    // signal: write the signal out
                    snprintf(death_message, 511, termed,
                             job->jid, job->pid, WTERMSIG(status));
                    signal_safe_write(death_message, strlen(death_message));
                    deletejob(jobs, job->pid);
                } else if (WIFEXITED(status)) {
                    // just clear the job
                    deletejob(jobs, job->pid);
                }
            }
        }
    }
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig) {
    // J. RANKIN drove here
    pid_t fgjob_pid = fgpid(jobs);
    if (fgjob_pid != 0) {
        kill(-fgjob_pid, SIGINT);
    }
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig) {
    // M. ELLIS drove here
    pid_t fgjob_pid = fgpid(jobs);
    if (fgjob_pid != 0) {
        kill(-fgjob_pid, SIGTSTP);
    }
}

/*********************
 * End signal handlers
 *********************/



/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) {
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) {
    ssize_t bytes;
    const int STDOUT = 1;
    bytes = write(STDOUT, "Terminating after receipt of SIGQUIT signal\n", 45);
    if (bytes != 45)
        exit(-999);
    exit(1);
}

/*
 * signal_safe_write - Perform a signal-handler-safe write to standard output.
 *    Originally provided in section 1.2 of specification.
 */
void signal_safe_write(const char *msg, size_t len) {
    ssize_t bytes;
    const int STDOUT = 1;
    bytes = write(STDOUT, msg, len);
    if (bytes != len)
        exit(-999);
}

