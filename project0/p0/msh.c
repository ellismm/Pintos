/* 
 * msh - A mini shell program with job control
 * 
 * <Put your name and login ID here>
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

#define ERR -1


/* Global variables */
int verbose = 0;            /* if true, print additional output */

extern char **environ;      /* defined in libc */
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
void signal_safe_write(const char *, ssize_t );



/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
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
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
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
void eval(char *cmdline) 
{
    char *argv[32];
    int is_bg = parseline(cmdline, argv);
    // J. RANKIN drove here
    if (!builtin_cmd(argv)) {
        // command not built in, run from the filesystem

		// block SIGCHLD signals until we finish generating the job
		sigset_t sigchld_only, old_set;
		sigemptyset(&sigchld_only);
		sigaddset(&sigchld_only, SIGCHLD);
		sigprocmask(SIG_BLOCK, &sigchld_only, &old_set);
		
		// create associated job
        pid_t fork_pid = fork();
        if (fork_pid == ERR) {
            perror("fork()");
        } else if (fork_pid == 0) {
            // child process (the one running the command)
			sigprocmask(SIG_UNBLOCK, &sigchld_only, &old_set);
            execv(argv[0], argv);

            if (errno == ENOENT) {
                fprintf(stdout, "%s: Command not found\n", argv[0]);
                exit(127);
            } else {
                unix_error(argv[0]);
                exit(1);
            }
        } else {
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
}


/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 * Return 1 if a builtin command was executed; return 0
 * if the argument passed in is *not* a builtin command.
 */
int builtin_cmd(char **argv) 
{
	// J. RANKIN drove here
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
void do_bgfg(char **argv) 
{
	if (argv[1] == NULL) {
		printf("%s command requires PID or %%jobid argument", argv[0]);
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
			printf("%s: No such job", job_id_s);
			return;
		}
	} else {
		// otherwise, job identified by process ID
		char *endptr = NULL;
		errno = 0;
		pid_t proc_id = (pid_t) strtol(job_id_s, &endptr, 10);
		if (errno != 0 || endptr == job_id_s) {
			printf("%s: argument must be a PID or %%jobid", argv[0]);
			return;
		}
		// lookup job
		tgt_job = getjobpid(jobs, proc_id);
		if (tgt_job == NULL) {
			printf("(%d): No such process", proc_id);
			return;
		}
	}

	if (strcmp(argv[0], "fg") == 0) {
	} else if (strcmp(argv[0], "bg") == 0) {
	}
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
	int status;
	pid_t wait_res = waitpid(pid, &status, WUNTRACED);
	if (wait_res == -1) {
	}
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
void sigchld_handler(int sig) 
{

    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
{
	char kill_message[64];
	pid_t get_pid = fgpid(jobs);
	if(get_pid != 0) {
		int job_id = pid2jid(jobs, get_pid);
		snprintf(kill_message, 63, "job [%d] (%d) terminated by signal %d\n", job_id, get_pid, SIGINT);
		kill_message[63] = 0;
		kill(get_pid, SIGINT);
	}
	signal_safe_write(kill_message, strlen(kill_message));
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
	char kill_message[64];
	pid_t get_pid = fgpid(jobs);
	if(get_pid != 0) {
		int job_id = pid2jid(jobs, get_pid);
		snprintf(kill_message, 63, "Job [%d] (%d) stopped by signal %d\n", job_id, get_pid, SIGTSTP);
		kill_message[63] = 0; 
		kill(get_pid, SIGTSTP);
	}
	signal_safe_write(kill_message, strlen(kill_message));
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
void usage(void) 
{
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
void sigquit_handler(int sig) 
{
    ssize_t bytes;
    const int STDOUT = 1;
    bytes = write(STDOUT, "Terminating after receipt of SIGQUIT signal\n", 45);
    if(bytes != 45)
       exit(-999);
    exit(1);
}

/**
 * Perform a signal-handler-safe write to standard output.
 * Originally provided in section 1.2 of specification
 */
void signal_safe_write(const char *msg, ssize_t len) {
    ssize_t bytes;
    const int STDOUT = 1;
    bytes = write(STDOUT, msg, len);
    if(bytes != len)
       exit(-999);
}

