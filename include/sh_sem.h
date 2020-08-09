#ifndef _SH_SEM_H_
#define _SH_SEM_H_ 1

#define SEMPERM 0600

typedef union _semun
{
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
} semun;


int sh_sem_lock(int   semid);
int sh_sem_init(key_t semkey);
int sh_sem_unlock(int   semid);

#endif /* _SH_SEM_H_ */

 
