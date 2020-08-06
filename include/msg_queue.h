
#ifndef _MSG_QUEUE__H
#define _MSG_QUEUE__H 1

#include <sys/types.h>
#include <sys/ipc.h>

#define MSQ_LEN 30

#define QKEY (key_t)0105
#define QPERM 0660
#define MAXOBN 50
#define MAXPRIOR 10

#define INT_LEN     10

typedef struct _msq_elem
{
  int p_id;
  int len;
  char msg[MSQ_LEN];
} msq_elem_t;

int init_queue();
int msg_send(msq_elem_t *msg_obj);
int msg_rcv(msq_elem_t *msg_obj);
void msg_free(msq_elem_t *msg_obj);
#endif