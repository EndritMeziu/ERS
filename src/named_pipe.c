#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include <sys/stat.h>

#include "named_pipe.h"

int init_named_pipe(char *pipe_name)
{
    if(mkfifo(pipe_name,0666) == -1)
    {
        if(errno != EEXIST){
            perror("Error creating the pipe.\n");
            exit(0);
        }
    }
    return 1;
}

int pipe_snd(char* pipe_name,named_pipe_t *pipe_obj)
{
    
    int fd;
    

    if((fd = open(pipe_name,O_WRONLY)) < 0)
    {
        perror("Fifo open failed.\n");
        exit(0);
    }
    
    printf("Sending message: %s\n",pipe_obj->msg);
    
    pipe_obj->len = strlen(pipe_obj->msg)+1;

    if(write(fd,&pipe_obj->len, 4) == -1)
    {
        perror("Error writing to pipe.\n");
        exit(0);
    } 

    if(write(fd,&pipe_obj->msg, pipe_obj->len) == -1)
    {
        perror("Error writing to pipe.\n");
        exit(0);
    } 
    close(fd);
    return 1;
}


int pipe_rcv(char* pipe_name,named_pipe_t *pipe_obj)
{

    int fd;
    if((fd = open(pipe_name,O_RDONLY)) < 0)
    {
        perror("Fifo open failed.\n");
        exit(0);
    }

    if(read(fd,&pipe_obj->len, 4) == -1)
    {
        perror("Error reading the pipe.\n");
        exit(0);
    }

    if(read(fd,&pipe_obj->msg, pipe_obj->len) == -1)
    {
        perror("Error reading the pipe.\n");
        exit(0);
    }
    close(fd);
    return 1;
}
