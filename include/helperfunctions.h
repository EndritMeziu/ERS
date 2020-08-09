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

#include "msg_queue.h"
#include "named_pipe.h"
#include "opt.h"
#include "patient.h"

void set_pipe_name(msq_elem_t *msgobj);
void snd_message(msq_elem_t msgobj,applOption aoObj,Patient_t patient);