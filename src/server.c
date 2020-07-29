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
#include "sh_mem.h"
#include "sh_sem.h"

int main (int argc, char *argv [])
{
  int r_qid;
  char buffer [SHM_MSG_LEN];
  msq_elem_t msg_obj;
  named_pipe_t npobj;
  shared_mem_t shm_obj;

  if((r_qid = init_queue()) == -1)
  {
      return (-1);
  }

  if (shm_init (&shm_obj) == -1)
    {
      perror ("Error initiating the shared memory");
      exit (-1);
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
    
    strncpy(buffer, npobj.msg,SHM_MSG_LEN);
    

      /* enter the critical section */
    if (sh_sem_lock (shm_obj.sem_id) == -1)
    {
      perror ("Error entering the shared memory critical section");
      exit (-1);
    }

    strcpy (&shm_obj.shm_ptr->buffer [0], &buffer [0]);
    shm_obj.shm_ptr->len = strlen (&buffer [0]);
    
    /* leave the critical section */
    
    (void) sh_sem_unlock (shm_obj.sem_id);

    /* reset the input buffer */
    memset (&buffer [0], 0x0, SHM_MSG_LEN);
    if (sh_sem_lock (shm_obj.sem_id) == -1)
    {
      perror ("Error entering the shared memory critical section");
      exit (EXIT_FAILURE);
    }

    if (shm_obj.shm_ptr->len > 0)
    {
      printf ("[message received]: '%s'\n", shm_obj.shm_ptr->buffer);

      /* reset the shared memory */
      memset (shm_obj.shm_ptr, 0x0, sizeof (shm_elm_t));
      
      /* leave the critical section */
      (void) sh_sem_unlock (shm_obj.sem_id);
    }

   
    
          
      
  }
}
