/**
   Akash Chaurasia
   601.220 (02)
   09/26/2017
   Homework 4
   achaura1
   achaura1@jhu.edu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

/**
   A function to give the numerical reprsentation of a disease status
   using the chars of the disease status

   @param t1 the string of chars representing the disease status
   @return the numerical representation of the disease status
*/

int int_dis_status(char t1[3]) {
  // returns values according to the assigned numerical values of the statuses
  if (strcmp(t1, "UN") == 0) {
    return 0;
  }
  if (strcmp(t1, "DF") == 0) {
    return 1;
  }
  if (strcmp(t1, "DN") == 0) {
    return 2;
  }
  if (strcmp(t1, "DT") == 0) {
    return 3;
  }
  return 4;
}


/**
   A function to load the diseease statuses in bits of the patient

   @param patients the array of patients
   @param t1 the disease status of HU
   @param t2 the disease status of FH
   @param t3 the disease status of SC
   @param t4 the disease status of TS
   @param t5 the disease status of CF
   @param index the index of the patient to load
*/
void load_diseases(unsigned int patients[], char t1[], char t2[], char t3[],
			char t4[], char t5[], int index) {
  // shift bits as necessary for disease statuses
  unsigned int d1 = int_dis_status(t1);
  d1 = d1 << 8;
  d1 += (int_dis_status(t2) << 6);
  d1 += (int_dis_status(t3) << 4);
  d1 += (int_dis_status(t4) << 2);
  d1 += int_dis_status(t5);
  patients[index] += d1;
  
}

/**
   A function to load a file's patient information in a
   patient array

   @param *fp a file pointer to the file to be read
   @param patients the array of patients
   @return number of patients entered (excludes duplicates)
*/

int load_data(FILE *fp, unsigned int patients[]) {

  // buffer for disease codes
  char t1[3], t2[3], t3[3], t4[3], t5[3];

  int i = 0;  // index                                                                                                                                                                                             

  // checks for duplicates and discard if new value is a duplicate
  int is_duplicate = 0;
  while (fscanf(fp, "%u", &patients[i]) == 1) {

    // scan the space
    is_duplicate = 0;
    for (int j = 0; j < i; j++) {
      unsigned int test_id = (patients[j] >> 17);
      if (test_id == patients[i]) {
	is_duplicate = 1;
	duplicate_entry((int) patients[i]);
	unsigned int scratch;
	char s1[3], s2[3], s3[3], s4[3], s5[3];
	fscanf(fp, "\t%u\t%s\t%s\t%s\t%s\t%s\n", &scratch, s1, s2, s3, s4, s5);
	patients[i] = 0;
      }
    }
    // load patient information if it is not a duplicate
    if (!(is_duplicate)) {
      
      unsigned int seventeen = 17;
      patients[i] = patients[i] << seventeen; 

      unsigned int temp_age;
      fscanf(fp, "\t%u\t%s\t%s\t%s\t%s\t%s\n", &temp_age, t1, t2, t3, t4, t5);

      // bit shift for age
      temp_age = temp_age << 10;
      patients[i] += temp_age;
      load_diseases(patients, t1, t2, t3, t4, t5, i);

      i++;
    }
  }
  // return number of patients loaded
  return i;
}


/**
   A function to load the file data and return the number of patients laoded
   @param *fp a pointer to the file to be read
   @param patients the array of patients
   @return the number of patients laoded
*/

int read_file(FILE *fp, unsigned int patients[]) {

  // call load function and return number loaded
  int num = load_data(fp, patients);

  
  return num;

}
