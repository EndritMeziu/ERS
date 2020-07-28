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


#define MSQ_LEN 30
#define NAMED_PIPE_MSG_LEN 512

#include "msg_queue.h"
#include "named_pipe.h"

int main (int argc, char *argv [])
{
  int r_qid;
  msq_elem_t msg_obj;
  named_pipe_t npobj;
  if((r_qid = init_queue()) == -1)
  {
      return (-1);
  }

  for(;;)
  {
    if(msg_rcv(&msg_obj)  == -1)
    {
        return -1;
    }
    
    printf("Data received:%d\n",msg_obj.p_id);
    printf("Data received:%d\n",msg_obj.len);
    printf("Data received:%s\n",msg_obj.msg);
    pipe_rcv(msg_obj.msg,&npobj);
    printf("Received message:%s\n",npobj.msg);
    /*printf("Pid:%d\n",msgobj.p_id);
    pipedata = readfifo(msgobj.msg);
    printf("Data on pipe:%s\n",pipedata);*/
          
      
  }
}
