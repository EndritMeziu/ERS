#ifndef _STR_SERIALIZE_H
#define _STR_SERIALIZE_H 1

#include <stdio.h>

#define SER_EOF         -2
#define SER_ALLOC_ERROR -1
#define SER_INT_LEN      4

typedef struct _CString
{
  int   length;
  char *p_str;
} CString_t;

/* public interface functions */
int str_len   (          /*@null@*/ const char      *      str);
int str_init  (          /*@null@*/ const char      *      str, CString_t *const c_str);
int str_write (FILE *fp, /*@null@*/ const CString_t *const c_str);
int str_read  (FILE *fp, /*@null@*/       CString_t *const c_str);

#endif

 
