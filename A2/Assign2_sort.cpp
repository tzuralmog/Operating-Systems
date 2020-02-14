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


    //Pipe variables
    char msg_buf[100] = "";
    int child_to_parent_pipe[2];
    int parent_to_child_pipe[2];
    int pfdout[2];

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
    if (pipe(pfdout) == -1)
    {
        perror("failed in file mimic pipe creation:");
        exit(9);
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
        execlp("sort", "sort","-k","3.3n","-k","1.1", "-k","2.2",  NULL); 
        // exec ls program 
    }

    

    int a = 0;
    a = open("./cs308a2_sort_data",O_RDONLY,0);


    //close all pipes other than child to parent read pipe to prevent block
    close(child_to_parent_pipe[1]);
    close(parent_to_child_pipe[0]);

    // reads sort output from child into other pipe
    
    int nread = 0;
    while ((nread  = read (a, msg_buf, 80))){
        write(parent_to_child_pipe[1],msg_buf,nread);
    }

    // close parent to child pipe
    close(parent_to_child_pipe[1]);

    // print bougus recived data.
    printf("\nAll Data Sent\n\n");

    // converts pipe to file
    FILE *fp = fdopen(child_to_parent_pipe[0], "r");

    // counts areacode occurences
    char lastname[100],firstname[100];
    int areacode, oldareacode = 0, prefix, telenum, areacount = 0, record = 0;


    while(fscanf(fp , "%s %s %d %d %d\n",lastname, firstname, &areacode, &prefix, &telenum) != EOF){
        if( oldareacode == 0){
            printf("First line = %s %s %d %d %d\n\n",lastname, firstname, areacode, prefix, telenum);
            oldareacode = areacode;
        }else if(oldareacode != areacode){
            
            printf("Area code %d occured %d times\n", oldareacode, areacount);
            oldareacode = areacode;
            areacount = 1;
        }else{
            areacount++;
        }
        record ++;
    }
    printf("Area code %d occured %d times\n", areacode, areacount);

    printf("\nAll Data Recievied\n%d RECORDS PROCCESED \n",record);


    exit(1);

}