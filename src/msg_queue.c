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

#include "msg_queue.h"
#include "named_pipe.h"
#define QKEY (key_t)0105

int init_queue()
{
    int queue_id;
    if((queue_id = msgget(QKEY,IPC_CREAT | QPERM)) == -1)
    {
        perror("Msgget failed.\n");
    }
    return (queue_id);
}

int msg_send(msq_elem_t *msg_obj)
{
    int s_qid;
    
    if((s_qid = init_queue()) == -1)
    {
        return -1;
    }

    
    if(msgsnd(s_qid, &msg_obj->p_id,INT_LEN,0) == -1)
    {
        perror("msgsnd failed.\n");
        return 0;
    }

    if(msgsnd(s_qid, &msg_obj->len,INT_LEN,0) == -1)
    {
        perror("msgsnd failed.\n");
        return 0;
    }

    if(msgsnd(s_qid, &msg_obj->msg,MSQ_LEN,0) == -1)
    {
        perror("msgsnd failed.\n");
        return 0;
    }
    printf("Msg:%s\n",msg_obj->msg);
    
    return 1;
}

int msg_rcv(msq_elem_t *msg_obj)
{
    int mlen,r_qid;
    
    if((r_qid = init_queue()) == -1)
    {
        return (-1);
    }
    

    if((mlen = msgrcv(r_qid,&msg_obj->p_id,INT_LEN,0,0)) == -1)
    {
        perror("Failed on receving the pid.\n");
        return (-1);
    }

    if((mlen = msgrcv(r_qid,&msg_obj->len,INT_LEN,0,0)) == -1)
    {
        perror("Failed on receving the length.\n");
        return (-1);
    }

    if((mlen = msgrcv(r_qid,&msg_obj->msg,MSQ_LEN,0,0)) == -1)
    {
        perror("Failed on receving the message.\n");
        return (-1);
    }
    return 1;

}

void msg_free(msq_elem_t *msg_obj)
{
    msg_obj->p_id = -1;
    msg_obj->len = -1;
    if(msg_obj->msg != NULL)
    {
        memset(msg_obj->msg,0,30);
    }
    
}