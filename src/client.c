
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



int main (int argc, char *argv [])
{
  applOption  aoObj;
  Patient_t    patient;
  msq_elem_t msgobj;
  char *pipe = "nmpiped_";
  char *mypid = malloc(8);
  sprintf(mypid, "%d", getpid());
  char *pipename = malloc(1+strlen(pipe)+strlen(mypid));
  strcat(pipename,pipe);
  strcat(pipename,mypid);
  msgobj.p_id = getpid();
  msgobj.len = strlen(pipename);
  strncpy(msgobj.msg,pipename,msgobj.len);
  msgobj.msg[msgobj.len] = '\0';

  init_named_pipe(msgobj.msg);
  /* initialize the application option object */
  opt_init (&aoObj);

  
  
  if ((char) opt_proc (argc, argv, &aoObj) == OPT_PROC_ERROR)
    {
      opt_free (&aoObj);
      return EXIT_FAILURE;
    }

  /* open a file */
  printf ("\nThe file '%s' is opening ... \n", aoObj.f_name);
  
  if ((aoObj.fp = fopen (aoObj.f_name, "a+")) == NULL)
  {
    printf ("\nError opening the file: '%s' [Error string: '%s']",
            aoObj.f_name, strerror (errno));
    opt_free (&aoObj);
    return -1;
  }
  

  

  msg_send(&msgobj);
  while(Patient_read(aoObj.fp,&patient) == 1)
  {  
    
    named_pipe_t npobj;
    char *message = malloc(512);
    strcpy(message,patient.ssec_no.p_str);
    /*sprintf(message,"%d %s %s %s %s %s",patient.id,patient.name.p_str,
    patient.surename.p_str,patient.age.p_str,patient.address.p_str,patient.date.p_str);
*/

    strncpy(npobj.msg, message, strlen(message));
    sleep(0.001);
    pipe_snd(msgobj.msg,&npobj);
    
    
    
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
  
  if (aoObj.list == (char) OPT_SPECIFIED)
    {
       
      
       printf ("\n\nThe content of the file: \n"); 
       
       /*Print every record of the file*/
       
       
     
           
    }

  /* release all resources */ 
  (void) opt_free (&aoObj);
   
  return 0;
}

