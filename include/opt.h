
#ifndef _OPT_H
#define _OPT_H 1_

#include <stdio.h>
#include <stdbool.h>

#define OPT_FILE         (char) 0x01
#define OPT_APPEND       (char) 0x02
#define OPT_MAX_NUM      (char) 0x04

#define OPT_PATTERN      "f:ahl"

#define INTA_APPEND      "append"
#define INTA_TIME        "time"
#define INTA_HELP        "help"
#define INTA_QUIT        "quit"
#define INTA_CMD_MAXLEN  100

#define OPT_SPECIFIED    (char) 0x01
#define OPT_NOTSPECIFIED (char) 0x02

#define OPT_SUCCESS      (char) 0x01
#define OPT_PROC_ERROR   (char) 0x02
#define OPT_MALLOCERROR  (char) 0x04
#define OPT_NULLPOINTER  (char) 0x08

typedef struct _applOption
{
  char  append;
  int  time;
  char *f_name;
  FILE *fp;
  
} applOption;

/* public interface functions */
void opt_init     (/*@out@*/applOption *aoObj);
void opt_free     (/*@out@*/applOption *aoObj);
int  opt_proc     (         int         argc, char *argv[], /*@out@*/applOption *aoObj);
int  opt_strclone (/*@out@*/char      **dest, const char *src);
void opt_usage    (   const char       *name);
bool isNumber(char number[]);
#endif /* _OPT_H */

 
