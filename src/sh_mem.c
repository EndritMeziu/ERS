#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "sh_mem.h"
#include "sh_sem.h"

int shm_init  (shared_mem_t *shm_obj)
{
  register int shm_flgs = 0666 | IPC_CREAT;

  /* init the shm_obj */
  shm_obj->sem_id  = -1;
  shm_obj->shm_id  = -1;
  shm_obj->shm_ptr = NULL;
  
  /* create the shared memory segment */
  if ((shm_obj->shm_id = shmget (SHM_KEY, sizeof (shm_elm_t), shm_flgs)) == -1)
    {
      if (errno == EEXIST)
        {
          shm_obj->shm_id = shmget (SHM_KEY, sizeof (shm_elm_t), 0666);
        }
      else
        {
          perror   ("Error: shmget failed -");
          
        }
      return -1;
    }

  /* attach the shared memory segment */
  shm_obj->shm_ptr = (shm_elm_t *) shmat (shm_obj->shm_id, 0, 0);
  if ((void *) shm_obj->shm_ptr == SHM_ERROR)
    {
      perror   ("Error: shmat failed");
      
      return -1;
    }

  shm_obj->shm_ptr->len = 0;
  shm_obj->shm_ptr->state = SHM_EMPTY;
  /* instantiate and initialize the shared memory semaphore object */
  if ((shm_obj->sem_id = sh_sem_init (SHS_KEY)) == -1)
    {
        perror   ("Error: sh_sem_init failed");
        
        return -1;
    }

  return 0;
}

void shm_free(shared_mem_t * shm_obj)
{
  shm_obj->sem_id = -1;
  shm_obj->shm_id = -1;
  shm_obj->shm_ptr->len = -1;

  if(shm_obj->shm_ptr->buffer != NULL)
    {
      memset(shm_obj->shm_ptr->buffer,0,512);
        
    }


}