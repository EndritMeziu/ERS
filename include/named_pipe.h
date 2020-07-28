#ifndef _NAMED_PIPE__H
#define _NAMED_PIPE__H 1

#include <sys/types.h>
#include <sys/ipc.h>

#define MSQ_LEN 30

#define QKEY (key_t)0105
#define QPERM 0660
#define MAXOBN 50
#define MAXPRIOR 10
#define NAMED_PIPE_MSG_LEN 512
#define INT_LEN     10

typedef struct named_pipe_
{
    int len;
    char msg[NAMED_PIPE_MSG_LEN];
} named_pipe_t;

int init_named_pipe(char *pipe_name);
int pipe_snd(char* pipe_name,named_pipe_t *pipe_obj);
int pipe_rcv(char* pipe_name,named_pipe_t *pipe_obj);

#endif