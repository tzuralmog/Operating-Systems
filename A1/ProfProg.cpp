/** This is the source file for the first assignment  **/
/** You may find it helpful to cut and paste from it  **/
/** The executable you need to exec has been compiled **/
/**   from this file and is located on mercury at:    **/
/**                 ~bill/cs308/Assign1               **/


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>

int counter = 0, counter_2G = 0;

void sig_handler(int signal){
	printf("\n\nPROF PROG: Awake in handler - You Got Me With Signal\n \
Number %d after spinning for more\n than %d %s loop iterations, goodbye\n\n", 
                signal, ((counter_2G)?counter_2G*2:counter),
                ((counter_2G)?"Billion":"\0"));
	exit(1);
}

int main(void)
{
	pid_t	pid, ppid;
	int	ruid, rgid, euid, egid;
	int	priority;
	sigset_t	mask, proc_mask;
	struct sigaction sigAc;

	sigemptyset(&proc_mask);                     /** clear signal mask **/
	sigprocmask(SIG_SETMASK, &proc_mask, NULL);

	
	sigemptyset(&mask);
	sigAc.sa_mask = mask;
	sigAc.sa_handler = sig_handler;
	sigAc.sa_flags = 0;

	if(sigaction(SIGTERM, &sigAc, NULL) == -1){
		perror("failed in sigaction:");
		exit(2);
	}

	pid = getpid();
	ppid = getppid();
	ruid = getuid();
	euid = geteuid();
	rgid = getgid();
	egid = getegid();
	if (nice(5) == -1){
                perror("set priority failed ");
                exit(2);
        }
        if((priority = getpriority(PRIO_PROCESS, 0)) == -1){
                perror("getpriority failed ");
                exit(2);
        }
        printf("\nThis is the Prof process report:\n");

	printf("\nThis is the Prof process report:\n");

	printf("\nPROF PROG:  Process ID is:\t\t%d\n\
PROF PROG:  Process parent ID is:\t%d\n\
PROF PROG:  Real UID is:\t\t%d\n\
PROF PROG:  Real GID is:\t\t%d\n\
PROF PROG:  Effective UID is:\t\t%d\n\
PROF PROG:  Effective GID is:\t\t%d\n\
PROF PROG:  Process priority is:\t%d\n\
", pid, ppid, ruid, rgid, euid, egid, priority);

	printf("\nPROF PROG: going into and endless loop, use\n \
kill %d   to kill me now\n\n",pid);
	fflush(stdout);

	while (counter_2G < 10){
		counter ++;
		if (counter < 0){
			counter = 0;
			counter_2G++;
		}
	}
	write(1,"PROF PROG: timed out after 20 billion iterations\n", 51);
	exit(2);
}