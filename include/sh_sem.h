#ifndef SH_SEM_H_
#define SH_SEM_H_ 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEMPERM 0600

typedef union _semun
{
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
} semun;

/* public interface functions */
int sh_sem_init   (key_t semkey);
int sh_sem_lock   (int   semid);
int sh_sem_unlock (int   semid);

#endif /* SH_SEM_H_ */

 
