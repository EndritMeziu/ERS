
#ifndef _SH_MEM__H
#define _SH_MEM__H 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* defines the shared memory constants */
#define SHS_KEY          (key_t)0x55EECCAA
#define SHM_KEY          (key_t)0xCCAAFFEE
#define SHM_ERROR        ((void *) -1)

#define SHM_EMPTY       (char)0x01
#define SHM_FULL        (char)0x02
#define SHM_MSG_LEN      (int) (512)

typedef struct _sh_mem_elm
{
  char state;
  int len;
  char buffer [SHM_MSG_LEN];
  
} shm_elm_t;

typedef struct _shared_mem
{
  int        sem_id;
  int        shm_id;
  shm_elm_t *shm_ptr;
  
} shared_mem_t;

/* public interface functions */
int  shm_init     (/*@out@*/ shared_mem_t * shm_obj);
void shm_free(shared_mem_t * shm_obj);
#endif /* _SH_MEM__H */

 
