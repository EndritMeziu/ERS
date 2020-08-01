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
#include<sys/wait.h>

#define MSQ_LEN 30
#define NAMED_PIPE_MSG_LEN 512

#include "msg_queue.h"
#include "named_pipe.h"
#include "sh_mem.h"
#include "sh_sem.h"

int main (int argc, char *argv [])
{

  
  int r_qid;
  char buffer [SHM_MSG_LEN];
  
  shared_mem_t shm_obj;
  msq_elem_t msg_obj;
  named_pipe_t npobj;
  
  if((r_qid = init_queue()) == -1)
  {
      return 0;
  }

  if (shm_init (&shm_obj) == -1)
  {
    perror ("Error initiating the shared memory");
    exit(0);
  }
  
  printf("Waiting...\n");
 
  pid_t pid;
  while(msg_rcv(&msg_obj) != -1)
  {
    printf("--------------------------------\n");
    printf("Got message from message queue..\n");
    printf("--------------------------------\n");
    switch(pid = fork())
    {
      case 0:
        for(;;)
        {
          pipe_rcv(msg_obj.msg,&npobj);
          strncpy(buffer, npobj.msg,SHM_MSG_LEN);
          printf("Mesazhi nga klienti:%s\n",npobj.msg);
          printf("Mesazhi nga klienti:%d\n",npobj.len);
        
          
          if(shm_obj.shm_ptr->state == SHM_EMPTY)
          {
            if (sh_sem_lock (shm_obj.sem_id) == -1)
            {
              perror ("Error entering the shared memory critical section");
              exit (0);
            }

            strcpy (&shm_obj.shm_ptr->buffer [0], &buffer [0]);
            shm_obj.shm_ptr->len = strlen (&buffer [0]);
            
            
            
            (void) sh_sem_unlock (shm_obj.sem_id);

          
            memset (&buffer [0], 0x0, SHM_MSG_LEN);
            shm_obj.shm_ptr->state = SHM_FULL;
          }
          if (sh_sem_lock (shm_obj.sem_id) == -1)
          {
            perror ("Error entering the shared memory critical section");
            exit (EXIT_FAILURE);
          }
          
          if (shm_obj.shm_ptr->len > 0)
          {
            printf ("[message received]: '%s'\n", shm_obj.shm_ptr->buffer);

            memset (shm_obj.shm_ptr, 0x0, sizeof (shm_elm_t));
            shm_obj.shm_ptr->state = SHM_EMPTY;
            
            (void) sh_sem_unlock (shm_obj.sem_id);
          }  
            
        }
        exit(1);
      default:
        break; 
    }
  }
  
  return 0;
  
}
