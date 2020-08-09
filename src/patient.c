#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include<stdbool.h>
#include "str_serialize.h"
#include "patient.h"

#define MAXPATIENTSTRLEN (512)

static 
int get_attr (CString_t *c_str, const char *attr_name)
{
  char  buff [MAXPATIENTSTRLEN];
  char *r_str  = NULL; 

  printf ("\n\tplease insert the %s: ", attr_name);
  r_str  = fgets (&buff[0], MAXPATIENTSTRLEN, stdin);

  return str_init (r_str, c_str);
}

int Patient_get (Patient_t *patient)
{
  time_t t;

  /* the patients CString_t shall  */
  patient->ssec_no.p_str = NULL;
  patient->person_id.p_str=NULL;
  patient->disease_id.p_str = NULL;
  patient->location_id.p_str = NULL;
  patient->date.p_str = NULL;
  patient->result = 0;
  printf ("\nPlease insert the patients information: ");
  
  /* Intializes random number generator */
  srand((unsigned) time(&t));

  if (get_attr  (&patient->ssec_no, "ssec_no") == 0)
    {
      perror("Initianization of social security number attribute failed");
      Patient_free(patient);
      return 0;
    }
  
  if (get_attr  (&patient->person_id, "person_id") == 0)
    {
      perror("Initianization of person id attribute failed");
      Patient_free(patient);
      return 0;
    }

  if(get_attr(&patient->disease_id, "disease_id") == 0)
    {
      perror("Initianization of age attribute failed");
      Patient_free(patient);
      return 0;
    }
  
  if (get_attr  (&patient->location_id, "location_id") == 0)
    {
      perror("Initianization of location id attribute failed");
      Patient_free(patient);
      return 0;
    }
  
  t = time(NULL);
  char * dateTested = ctime(&t);
  int status = str_init(dateTested,&patient->date);
  if(status == 0)
    { 
      perror("Initianization of date attribute failed");
      Patient_free(patient);
      return 0;
    }
  
  return 1;

}

void Patient_free(Patient_t *patient)
{

    if(patient->ssec_no.p_str != NULL)
    {
        free(patient->ssec_no.p_str);
        patient->ssec_no.p_str = NULL;
    }

    if(patient->person_id.p_str != NULL)
    {
        free(patient->person_id.p_str);
        patient->person_id.p_str = NULL;
    }

    if(patient->disease_id.p_str != NULL)
    {
        free(patient->disease_id.p_str);
        patient->disease_id.p_str = NULL;
    }

    if(patient->location_id.p_str != NULL)
    {
        free(patient->location_id.p_str);
        patient->location_id.p_str = NULL;
    }

    if(patient->date.p_str != NULL)
    {
        free(patient->date.p_str);
        patient->date.p_str = NULL;
    }

    /*patient.result?*/
    
}

int Patient_write (FILE *fp,  Patient_t *patient)
{
  int status = 0;
  int bresult = 0;
  if (patient == NULL)
    {
      return status;
    }
 
  status = (int) str_write (fp, &patient->ssec_no);
  if (status == 0)
    {
       printf ("\nError writing patient's social security number '%s' to the file", patient->ssec_no.p_str);
       return status;
    }
  

  status = (int) str_write (fp, &patient->person_id);
  if (status == 0)
    {
       printf ("\nError writing patient's id '%s' to the file", patient->person_id.p_str);
       return status;
    }

  status = (int) str_write (fp, &patient->disease_id);
  if (status == 0)
    {
       printf ("\nError writing disease id: '%s' to the file", patient->disease_id.p_str);
       return status;
    }

  status = (int) str_write (fp, &patient->location_id);
  if (status == 0)
    {
       printf ("\nError writing location id: '%s' to the file", patient->location_id.p_str);
       return status;
    }

  bresult = patient->result;
  status = (int) fwrite ((const void *) &bresult, (size_t) 1, (size_t) SER_INT_LEN, fp);

  if (status == 0)
    {
       printf ("\nError writing patient result to the file");
       return status;
    }

  
  status = (int) str_write (fp, &patient->date);
  if (status == 0)
    {
       printf ("\nError writing patient's date '%s' to the file", patient->date.p_str);
       return status;
    }
  Patient_free(patient);
  return status;
}

int Patient_read (FILE *fp,  Patient_t *const patient)
{
  int   status = 0;
  int   bresult   = 0;
  if (fp == NULL || patient == NULL) 
    {
      return status;
    }


  status = (int) str_read (fp, &patient->ssec_no);
  if (status == 0)
    {
       printf ("\nError reading patient's social security number '%s' to the file", patient->ssec_no.p_str);
       return status;
    }
  
  status = (int) str_read (fp, &patient->person_id);
  if (status == 0)
    {
       printf ("\nError reading patient's id '%s' to the file", patient->person_id.p_str);
       return status;
    }

  status = (int) str_read (fp, &patient->disease_id);
  if (status == 0)
    {
       printf ("\nError reading disease id: '%s' to the file", patient->disease_id.p_str);
       return status;
    }
  
  status = (int) str_read (fp, &patient->location_id);
  if (status == 0)
    {
       printf ("\nError reading location id: '%s' to the file", patient->location_id.p_str);
       return status;
    }
  
  status = fread ( (void *) &bresult, (size_t) 1, (size_t) SER_INT_LEN, fp);
  if (status == 0)
  {
    if (feof (fp) != 0)
    {
      return SER_EOF;
    }
    else
    {
      printf ("\nError reading from file");
      return status;
    }
  }
  
  patient->result = bresult;

  status = (int) str_read (fp, &patient->date);
  if (status == 0)
    {
       printf ("\nError reading patient's date from the file");
       return status;
    }
    
  return 1;
}
