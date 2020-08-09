#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>
#include <stdbool.h>
/*@-skipposixheaders@*/
#include <sys/types.h>
#include <unistd.h>

#define MSQ_LEN 30
#define NAMED_PIPE_MSG_LEN 512

#include "msg_queue.h"
#include "named_pipe.h"
#include "sh_mem.h"
#include "sh_sem.h"

#include "opt.h"
#include "str_serialize.h"
#include "patient.h"

int main (int argc, char *argv [])
{

  
  int r_qid;
  char buffer [SHM_MSG_LEN];
  
  shared_mem_t shm_obj;
  msq_elem_t msg_obj;
  named_pipe_t npobj;
  applOption aoObj;
  
  opt_init (&aoObj);

   if ((char) opt_proc (argc, argv, &aoObj) == OPT_PROC_ERROR)
  {
    opt_free (&aoObj);
    return EXIT_FAILURE;
  }

  
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
 
  int pid;
  int previouspid = -1;
  int status;
  while(msg_rcv(&msg_obj) != -1)
  {
    printf("----------------------------------\n");
    printf("  Got message from message queue\n");
    printf("----------------------------------\n");

    switch(pid = fork())
    {
      case 0:
          /*conn_handler*/
        while(true)
        {
          
          if((pipe_rcv(msg_obj.msg,&npobj)) != 1)
          {
            perror("Error receiving data forom pipe.\n");
            exit(0);
          }
          strncpy(buffer, npobj.msg,SHM_MSG_LEN);
            /*Place in shared memory*/
          
          while(true)
          {
            if(shm_obj.shm_ptr->state == SHM_EMPTY)
            {
              
              if (sh_sem_lock (shm_obj.sem_id) == -1)
              {
                perror ("Error entering the shared memory critical section");
                exit (0);
              }
              strcpy (&shm_obj.shm_ptr->buffer [0], &buffer [0]);
              shm_obj.shm_ptr->len = (int)strlen (&buffer [0]);  
              
              (void) sh_sem_unlock (shm_obj.sem_id);
              memset (&buffer [0], 0x0, SHM_MSG_LEN);
              
              if(strcmp(npobj.msg,"END") != 0)
                shm_obj.shm_ptr->state = SHM_FULL;
              else
                previouspid = pid;
              break;
              
            }
            
          }
          
          if(strcmp(npobj.msg,"END") == 0)
          {
            
            printf("Arrived at the end conn_handler.\n");
            shm_obj.shm_ptr->state = SHM_FULL;
            break;
          }
          
        }
        exit(0);
      default:
        break; 
    }
    if ((aoObj.fp = fopen ("fileser.txt", "a+")) == NULL)
    {
      printf ("\nError opening the file: '%s' [Error string: '%s']",
              aoObj.f_name, strerror (errno));
      opt_free (&aoObj);
      return -1;
    }

    while(true)
    {
      if(shm_obj.shm_ptr->state == SHM_FULL)
      {
        if(strcmp(shm_obj.shm_ptr->buffer,"END") == 0)
        {
           printf("Arrived at the end server.\n");
           (void) waitpid(previouspid,&status,0);
           shm_obj.shm_ptr->state = SHM_EMPTY;
          break;
        }

        if (sh_sem_lock (shm_obj.sem_id) == -1)
        {
          perror ("Error entering the shared memory critical section");
          exit (EXIT_FAILURE);
        }
        if (shm_obj.shm_ptr->len > 0)
        {
          printf ("[message received]: '%s'\n", shm_obj.shm_ptr->buffer);
          
          fprintf(aoObj.fp,"%s",shm_obj.shm_ptr->buffer);
          memset (shm_obj.shm_ptr, 0x0, sizeof (shm_elm_t));
          
          (void) sh_sem_unlock (shm_obj.sem_id);
          shm_obj.shm_ptr->state = SHM_EMPTY;
        }
        
      }
      
    }
    (void)fclose(aoObj.fp);
    opt_free (&aoObj);

    
  } 
  msg_free(&msg_obj);
  shm_free(&shm_obj);
  (void)fclose(aoObj.fp);
  opt_free (&aoObj);
  return 0;
  
}
