
#ifndef _PATIENT_H
#define _PATIENT_H 1

#include <stdio.h>
#include<stdbool.h>
#include "str_serialize.h"

/* struct to define Patient object*/
typedef struct _Patient
{
  CString_t ssec_no;
  CString_t person_id;
  CString_t disease_id;
  CString_t location_id;
  bool result;
  CString_t date;
} Patient_t;

/* public interface functions */
int Patient_get   (Patient_t *patient);
void Patient_free(Patient_t *patient);
int Patient_write (FILE *fp,      Patient_t *patient);
int Patient_read  (FILE *fp,      Patient_t *const patient);
#endif