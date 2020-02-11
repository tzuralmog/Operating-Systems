// Tzurs Assignment 1

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/wait.h>

int counter = 0, counter_2G = 0;


typedef union{
    int exit_status;
    struct{
        unsigned  sig_num:7;
        unsigned  core_dmp:1;
        unsigned  exit_num:8;
    }parts;
}LE_Wait_Status;

void sig_handler(int signal){
	printf("\n\nCHILD PROC: Awake in handler - You Got Me With Signal\n \
Number %d after spinning for more\n than %d %s loop iterations\n\n", 
                signal, ((counter_2G)?counter_2G*2:counter),
                ((counter_2G)?"Billion":"\0"));

    printf("Loading prof program");
    fflush(stdout);
    execl("./ProfProg","ProfProg",NULL);
	exit(1);
}

int main(void)
{

    //Local variables
    pid_t pid, ppid;
    int ruid, rgid, euid, egid;
    int priority;
    char msg_buf[10];
    int sync_pipe[2];

    // use the pipe() system call to create the pipe

    if (pipe(sync_pipe) == -1)
    {
        perror("failed in Parent pipe creation:");
        exit(7);
    }

    // use various system calls to collect and print process details

    printf("\nThis is the Parent process report:\n");
    pid = getpid();
    ppid = getppid();
    ruid = getuid();
    euid = geteuid();
    rgid = getgid();
    egid = getegid();
    priority = getpriority(PRIO_PROCESS, 0);

    printf("\nPARENT PROG:  Process ID is:\t\t%d\n\
PARENT PROC:  Process parent ID is:\t%d\n\
PARENT PROC:  Real UID is:\t\t%d\n\
PARENT PROC:  Real GID is:\t\t%d\n\
PARENT PROC:  Effective UID is:\t\t%d\n\
PARENT PROC:  Effective GID is:\t\t%d\n\
PARENT PROC:  Process priority is:\t%d\n",
           pid, ppid, ruid, rgid, euid, egid, priority);

    fflush(stdout);

/*
    Fork to create child
*/

    switch (pid = fork())
    {
    case -1: // if the call failes
            perror("Parent cannot create child");
        exit(4);

    default: // this is the parent's case
        printf("\nPARENT PROG: created Child with %d PID\n",pid);
        break;

    case 0: // child case

        // gets mask from system
        sigset_t	mask, proc_mask;
        // makes new handler
	    struct sigaction sigHandler;
        // LE_Wait_Status exit_union;

        // Empties previous mask
        sigemptyset(&proc_mask);
	    sigprocmask(SIG_SETMASK, &proc_mask, NULL);

	
	    sigemptyset(&mask);
	    sigHandler.sa_mask = mask;
	    sigHandler.sa_handler = sig_handler;
	    sigHandler.sa_flags = 0;

	    if(sigaction(SIGTERM, &sigHandler, NULL) == -1){
	    	perror("failed in sigaction:");
	    	exit(2);
	    }

        //  print information
        printf("\nThis is the Child process report:\n");
        pid = getpid();
        ppid = getppid();
        ruid = getuid();
        euid = geteuid();
        rgid = getgid();
        egid = getegid();
        priority = getpriority(PRIO_PROCESS, 0);

        printf("\nCHILD PROG:  Process ID is:\t\t%d\n\
CHILD PROC:  Process parent ID is:\t%d\n\
CHILD PROC:  Real UID is:\t\t%d\n\
CHILD PROC:  Real GID is:\t\t%d\n\
CHILD PROC:  Effective UID is:\t\t%d\n\
CHILD PROC:  Effective GID is:\t\t%d\n\
CHILD PROC:  Process priority is:\t%d\n",
               pid, ppid, ruid, rgid, euid, egid, priority);

        // write to parent
        fflush(stdout);
        write(sync_pipe[1], "x", 10);

        // enter endless loop and wait for parent to terminate
        while (true){
		    counter ++;
		    if (counter < 0){
			counter = 0;
			counter_2G++;
		    }
	    }
    } // switch and child end

    
    read(sync_pipe[0], msg_buf, 10);
    printf("\nPARENT PROG: Killing child\n");
    fflush(stdout);
    kill(pid, SIGTERM);

    LE_Wait_Status Exit;
    wait(&Exit.exit_status);
   
    printf(" PARENT PROG: Child Stats=\n\
    Exit status: %d\n\
    Exit signal: %d\n\
    Core Dump: %d\n\
    Exit Number: %d\n", Exit.exit_status , Exit.parts.sig_num,Exit.parts.core_dmp,Exit.parts.exit_num); 


    printf("\nPARENT PROG: Child terminated by %s %s a core dump\n", ((Exit.parts.sig_num)?"signal":"exit"),((Exit.parts.core_dmp)?"with":"without"));

    exit(0);
}