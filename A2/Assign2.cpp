// Tzurs Assignment 2

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <fcntl.h>

int main( int argc, char *argv[] ){

    int i = 0;
    while(i < argc){
        printf("\nArgv %d = %s\n",i,argv[i]);
        i++;
    }

    //Pipe variables
    char msg_buf[100];
    int sync_pipe[2];

    // use the pipe() system call to create the pipe

    if (pipe(sync_pipe) == -1)
    {
        perror("failed in Parent pipe creation:");
        exit(7);
    }

    if(fork() == 0){ 
        // create child to run ls   
        close(0);      
        open("./grep1", O_RDONLY);
        close(1);      
        dup(sync_pipe[1]);
        close(sync_pipe[0]);
        close(sync_pipe[1]);      
        execlp("grep", "grep", "123", NULL); 
        // exec ls program 
    }

    //close write pipe to prevent block
    close(sync_pipe[1]);
    
    //init newline counter
    int newLine = 0;

    // print bougus recived data.
    printf("All Data Sent\n\n");

    // parent reads
    while(read(sync_pipe[0], msg_buf, 1)){
        printf("%s",msg_buf);
        if (msg_buf[0] == '\n'){
            newLine++;
        }
    }

    printf("Total matching line count: %d\n", newLine);
    exit(1);

}