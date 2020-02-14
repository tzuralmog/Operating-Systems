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

    //Incoming file
    char filePoint[100];
    filePoint[0]='.';
    filePoint[1]='/';
    for (int i = 0; i < 80; i++) { 
        filePoint[i+2] = argv[1][i]; 
    } 

    //Pipe variables
    char msg_buf[100] = "";
    int child_to_parent_pipe[2];
    int parent_to_child_pipe[2];

    // use the pipe() system call to create the pipes
    if (pipe(child_to_parent_pipe) == -1)
    {
        perror("failed in Child to Parent pipe creation:");
        exit(7);
    }
    if (pipe(parent_to_child_pipe) == -1)
    {
        perror("failed in Parent to Child pipe creation:");
        exit(8);
    }

    if(fork() == 0){ 
        // create child to run ls   
        close(0);      
        dup(parent_to_child_pipe[0]);
        close(1);      
        dup(child_to_parent_pipe[1]);
        close(child_to_parent_pipe[0]);
        close(child_to_parent_pipe[1]);
        close(parent_to_child_pipe[0]);
        close(parent_to_child_pipe[1]);     
        execlp("grep", "grep", "-a" ,"123", NULL); 
        // exec ls program 
    }

    // parent reads from file
    FILE *fp = fopen(filePoint, "r");

    // init bug testing counter
    int starcount = 0;

    // parent sends to child info
    while(fgets(msg_buf, 80,fp) != NULL){
        write(parent_to_child_pipe[1], msg_buf, 80);
        printf("*");
        // printf("%s",msg_buf);
        starcount++;
        if(starcount %80 == 0){
            printf("      Total stars = %d : Total data in bytes = %d\n",starcount,starcount*80);
        }
    }
    printf("      Total stars = %d : Total data in bytes = %d\n",starcount,starcount*80);


    //close all pipes other than child to parent read pipe to prevent block
    close(child_to_parent_pipe[1]);
    close(parent_to_child_pipe[0]);
    close(parent_to_child_pipe[1]);
    
    //init newline counter
    int newLine = 0;

    // print bougus recived data.
    printf("\nAll Data Sent\n\n");

    // makes new char buffer to accept info from child pipe
    char msg_bufn[100] = "";

    // parent reads
    while(read(child_to_parent_pipe[0], msg_bufn, 1) ){
        printf("%s",msg_bufn);
        if(msg_bufn[0] == '\n'){
            newLine++;
        }

    }

    printf("\nTotal matching line count: %d\n", newLine);
    exit(1);

}