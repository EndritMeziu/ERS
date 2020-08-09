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
#include <stdio.h>

#include "msg_queue.h"
#include "named_pipe.h"
#include "opt.h"
#include "patient.h"

void set_pipe_name(msq_elem_t *msgobj)
{
  char *pipe = "nmpiped_";
  char *mypid = malloc(8);
  memset(mypid, 0, 8);
  sprintf(mypid,"%d", getpid());
  char *pipename = malloc(1+strlen(pipe)+strlen(mypid));
  memset(pipename, 0, 1+strlen(pipe)+strlen(mypid));
  strcat(pipename,pipe);
  strcat(pipename,mypid);
  free(mypid);
  msgobj->p_id = getpid();
  msgobj->len = strlen(pipename);
  
  if(pipename != NULL && msgobj->len > 0)
  {
    strncpy(msgobj->msg,pipename,msgobj->len);
    msgobj->msg[msgobj->len] = '\0';
  }
  else{
    memset (msgobj->msg, 0, MSQ_LEN );
  }
  free(pipename);
}

void snd_message(msq_elem_t msgobj,applOption aoObj,Patient_t patient)
{
  named_pipe_t npobj;
  char *message = malloc(512);
  sprintf(message,"%s %s %s %s %d %s",patient.ssec_no.p_str,patient.person_id.p_str,
  patient.disease_id.p_str,patient.location_id.p_str,(int)patient.result,patient.date.p_str);
  strncpy(npobj.msg, message, 512);

  if(aoObj.time == 1)
    (void)sleep(1);
  else
    (void)sleep(aoObj.time);

  (void)  pipe_snd(msgobj.msg,&npobj);
  free(message);
  Patient_free(&patient);
}