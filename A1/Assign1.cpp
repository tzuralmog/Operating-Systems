//
// Help file for cs305 Assignment 7
//
// This file has some code any many comments to
// direct you through assignment #7
//

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/resource.h>

// need just a main function, no args

int main(void)
{

    // some local variables

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

    printf("\nPARENT PROC: will now create child, write pipe,\n \
and do a normal termination\n");

    // now use the fork() call to create the child:
    //
    // format is:
    //
    switch (pid = fork())
    {
    case -1: // if the call failes
            perror("Parent cannot create child");
        exit(4);

    default: // this is the parent's case
             // parent must write message to pipe and
             // do a normal exit
        printf("\nPARENT PROG: created Child with %d PID\n",pid);
        read(sync_pipe[0], msg_buf, 10);
        printf("\nPARENT PROG: Succsess\n");
        break;

    case 0: // this is the child's case
            // child must create and print report
            // child must read pipe message and print
            // a modified version of it to output
            // child must do a normal exit
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

        //---------------------------------
    write(sync_pipe[1], "x", 10);

        printf("CHILD PROC: Process parent ID now is:   %d\n",
               getppid());
    } // switch and child end


    exit(0);
}