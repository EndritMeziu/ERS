

#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "sh_sem.h"

int sh_sem_init  (key_t semkey)
{
  int status = 0;
  int semid  = -1;
  
  if ((semid = semget (semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL)) == -1)
    {
      if (errno == EEXIST)
        {
          semid = semget (semkey, 1, 0);
        }
    }
  else
    {
      semun semopts;    
      semopts.val = 1;
      semctl (semid, 0, SETVAL, semopts);
    }
  

  if (semid == -1 || status == -1)
    {
      perror ("sh_sem_init failed");
      return (-1);
    }


  return semid;
}

int sh_sem_lock  (int semid)
{
  struct sembuf p_buf;
  p_buf.sem_num = 0;
  p_buf.sem_op  = -1;
  p_buf.sem_flg = SEM_UNDO;
  
  if (semop (semid, &p_buf, 1) == -1)
    {
      perror ("sh_sem_lock failed");
      return (-1);
    }

  return (0);
}

int sh_sem_unlock  (int semid)
{
  struct sembuf v_buf;
  v_buf.sem_num = 0;
  v_buf.sem_op  = 1;
  v_buf.sem_flg = SEM_UNDO;
  
  if (semop (semid, &v_buf, 1) == -1)
    {
      perror ("sh_sem_unlock failed");
      return (-1);
    }

  return (0);
}
