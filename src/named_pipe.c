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
        perror("Error creating fifo.\n");
        exit(-1);
    }
    return 1;
}

int pipe_snd(char* pipe_name,named_pipe_t *pipe_obj)
{
    int fd;
    char *message = "Message from client";
    if(init_named_pipe(pipe_name) == -1)
    {
        exit(-1);
    }

    if((fd = open(pipe_name,O_WRONLY)) < 0)
    {
        perror("Fifo open failed.\n");
        exit(-1);
    }

    pipe_obj->len = strlen(message);
    strncpy(pipe_obj->msg, message, pipe_obj->len);
    
    printf("Sending message: %s\n",pipe_obj->msg);
    
    if(write(fd,&pipe_obj->msg, NAMED_PIPE_MSG_LEN) == -1)
    {
        perror("Error writing to pipe.\n");
        exit(1);
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
        exit(-1);
    }
    
    if(read(fd,&pipe_obj->msg, NAMED_PIPE_MSG_LEN) == -1)
    {
        perror("Error reading the pipe.\n");
        exit(1);
    }

    close(fd);
    
    return 1;
}
