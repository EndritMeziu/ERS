
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

#define QKEY (key_t)0105
#define QPERM 0660
#define MAXOBN 50
#define MAXPRIOR 10

#define MSQ_LEN 30
#define NAMED_PIPE_MSG_LEN 512

#include "msg_queue.h"
#include "named_pipe.h"



int main (int argc, char *argv [])
{
  msq_elem_t msgobj;
  named_pipe_t npobj;
  msg_send(&msgobj);
  printf("Klienti: %s\n",msgobj.msg);
  pipe_snd(msgobj.msg,&npobj);
  return 0;
}

