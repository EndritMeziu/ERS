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

#define QKEY (key_t)0105
#define QPERM 0660
#define MAXOBN 50
#define MAXPRIOR 10

#define MSQ_LEN 30
#define NAMED_PIPE_MSG_LEN 512

#include "msg_queue.h"
#include "named_pipe.h"

#include "opt.h"
#include "str_serialize.h"
#include "patient.h"

void snd_message(msq_elem_t msgobj,applOption aoObj,Patient_t patient)
{
  named_pipe_t npobj;
  char *message = malloc(512);
  sprintf(message,"%s %s %s %s %d %s",patient.ssec_no.p_str,patient.person_id.p_str,
  patient.disease_id.p_str,patient.location_id.p_str,patient.result,patient.date.p_str);
  strncpy(npobj.msg, message, 512);

  if(aoObj.time == 1)
    sleep(1);
  else
    sleep(aoObj.time);

  pipe_snd(msgobj.msg,&npobj);
  free(message);
  Patient_free(&patient);
}

void set_pipe_name(msq_elem_t *msgobj)
{
  char *pipe = "nmpiped_";
  char *mypid = malloc(8);
  memset(mypid, 0, 8);
  sprintf(mypid, "%d", getpid());
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


int main (int argc, char *argv [])
{
  applOption  aoObj;
  Patient_t    patient;
  msq_elem_t msgobj;
 
  opt_init (&aoObj);

  if ((char) opt_proc (argc, argv, &aoObj) == OPT_PROC_ERROR)
  {
    opt_free (&aoObj);
    free(argv);
    return EXIT_FAILURE;
  }


  set_pipe_name(&msgobj);

  init_named_pipe(msgobj.msg);
  /* initialize the application option object */
  
  if(aoObj.f_name != NULL)
  {
    if ((aoObj.fp = fopen ("file.txt", "a+")) == NULL)
    {
      printf ("\nError opening the file: '%s' [Error string: '%s']",
              aoObj.f_name, strerror (errno));
      opt_free (&aoObj);
      return -1;
    }
  }
  else
  {
    opt_free (&aoObj);
    return -1;
  }
  if (aoObj.append == (char) OPT_SPECIFIED)
    {
      /* append a patient */
      if (Patient_get (&patient) == 0)
        {
          printf ("\nError getting the patient ");
          (void) opt_free (&aoObj);
          return -1;
        }
         
      if (Patient_write (aoObj.fp, &patient) == 0)
        {
          printf ("\nError writing the patient");
          (void) opt_free    (&aoObj);
          (void) Patient_free (&patient);
          return -1;
        }
    }

  msg_send(&msgobj);
  
  while(Patient_read(aoObj.fp,&patient) == 1)
  {  
    
    snd_message(msgobj,aoObj,patient);
    
  }
  named_pipe_t npobj;

  char *finalMsg = "END";
  strncpy(npobj.msg, finalMsg, 512);
  if(aoObj.time == 1)
    sleep(1);
  else
    sleep(aoObj.time);
  pipe_snd(msgobj.msg,&npobj);
  
  /* release all resources */ 
  opt_free (&aoObj);
  return 0;
}


