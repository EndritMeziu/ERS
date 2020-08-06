#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <getopt.h>
#include <unistd.h>
#include <libgen.h>

#include "opt.h"

extern char *optarg;
extern int   optopt;

int opt_check__  (/*@out@*/applOption *aoObj)
{
  int res = (int) OPT_SUCCESS;

  if (aoObj == NULL)
    {
      return (int) OPT_PROC_ERROR;
    }
  

  if (aoObj->f_name == NULL)
    {
      printf ("\nThe file option (-f f_name) has to be defined\n");
      res = (int) OPT_PROC_ERROR;
    }

  return res;
}

void opt_init  (/*@out@*/applOption *aoObj)
{
  if (aoObj == NULL)
    {
      return;
    }
  
  aoObj->append      = OPT_NOTSPECIFIED;
  aoObj->list        = OPT_NOTSPECIFIED;
  aoObj->f_name      = NULL;
  aoObj->fp          = NULL;
}

void opt_free  (/*@out@*/applOption *aoObj)
{
  if (aoObj == NULL)
    {
      return;
    }

  if (aoObj->f_name == NULL || aoObj->fp == NULL)
    {
      return;
    }
  
  aoObj->append      = OPT_NOTSPECIFIED;
  aoObj->list        = OPT_NOTSPECIFIED;

  if (aoObj->f_name != NULL)
    {
      free ((void *) aoObj->f_name);
      aoObj->f_name = NULL;
    }

  if (aoObj->fp != NULL)
    {
      (void) fclose (aoObj->fp);
      aoObj->fp = NULL;
    }
}

int opt_proc (int argc, char *argv [], /*@out@*/applOption *aoObj)
{
  int n_opt = 0;

  if (aoObj == NULL)
    {
      return (int) OPT_PROC_ERROR;
    }
  
  /* while ((n_opt = getopt (argc, argv, OPT_PATTERN)) != -1) */
  while ((n_opt = getopt (argc, argv, "af:hl")) != -1)
    {
      switch (n_opt)
        {
        case 'f':
          if (opt_strclone (&aoObj->f_name, optarg) == (int) OPT_NULLPOINTER)
            {
              return (int) OPT_PROC_ERROR;
            }
          break;
          
        case 'a':
          aoObj->append = OPT_SPECIFIED;
          break;
          
        case 'h':          
          opt_usage (argv [0]);
          break;
          
        case 'l':
          aoObj->list = OPT_SPECIFIED;
          break;
          
        case '?':
          printf ("\nUnsupported option '%c'", n_opt);
          opt_usage (argv [0]);
          break;
          
        default: /* '?' */
          opt_usage (argv [0]);  
        }
     
    }

  if (opt_check__ (aoObj) == (int) OPT_PROC_ERROR)
    {
      opt_usage (argv [0]);
      return (int) OPT_PROC_ERROR;
    }

  return (int) OPT_SUCCESS;
}

int opt_strclone (/*@out@*/ char **dest, const char *src)
{
  size_t  s_len = 0;
  char   *ptr   = NULL;
  
  /* check preconditions - given parameters are not equal to NULL */
  if (dest == NULL || src == NULL)
    {
      return (int) OPT_NULLPOINTER;
    }

  s_len = strlen (src);
  ptr   = (char *) malloc (s_len +1);

  if (ptr == NULL)
    {
      return (int) OPT_MALLOCERROR;
    }

  *dest = ptr;

  while (*src != '\0')
    {
      *ptr++ = *src++;
    }
  
  return (int) OPT_SUCCESS;
}

void opt_usage (const char *app_name)
{
  char *a_name = basename ((char *) app_name);
  
  printf ("\n%s - The first exercise in the sys prog lecture\n", a_name);
  printf ("\nSYNOPSIS\n");
  printf ("\t%s [OPTION] ... \n", a_name);
  printf ("\t -f arg, (mandatory) the file name \n");
  printf ("\t -a,     (optional) append a patient to the file \n");
  printf ("\t -l,     (optional) list all patients stored in the file \n\n");

  exit (EXIT_FAILURE);
}
