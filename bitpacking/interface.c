/**
 * Implementation of the user interface functions
 * as described in the interface header file
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#define NUM_DISEASES 5

static const char *disease_codes[] = {"HU", "FH", "SC", "TS", "CF"};

void print_menu() {
  printf(
         "n - display total number of patients in the database\n"
         "d - get the disease profile of a patient\n"
         "e - edit disease profile of a patient\n"
         "c - get the disease profile of crossover between two patients\n"
         "l - get IDs and average age of all patients suffering from a list of"
         "given diseases\n"
         "s - save the database to a file\n"
         "q - quit the program\n"
         "Enter letter choice -> \n");
}

void output_n(int num_patients) {
  printf("Total number of patients: %d\n", num_patients);
}

void prompt_d() {
  printf("Enter a patient ID -> \n");
}

void prompt_e() {
  printf("Enter space separated patient ID, disease -> \n");
}

void prompt_c() {
  printf("Enter two space separated patient IDs -> \n");
}

void prompt_l() {
  printf("Enter space separated disease codes -> \n");
}

void prompt_s() {
  printf("Enter filename -> \n");
}

void duplicate_entry(int id) {
  printf("Duplicate entry %d discarded\n", id);
}

void bad_patient_ID(int id) {
  printf("Bad patient ID %d\n", id);
}

void bad_disease_code(char *code) {
  printf("Bad disease code %s\n", code);
}

void bad_menu_option(char option) {
  printf("Bad menu option %c\n", option);
}

void output_disease_profile(int disease_profile[]) {

  char *status;
  for (int i = 0; i < NUM_DISEASES; i++) {
    switch(disease_profile[i]) {
    case 0:
      status = "Unknown";
      break;
    case 1:
      status = "Disease-free";
      break;
    case 2:
      status = "Disease, not being treated";
      break;
    case 3:
      status = "Disease, being treated";
      break;
    default:
      fprintf(stderr, "Invalid disease status code\n");
      exit(-1);
    }
    printf("%s: %s\n", disease_codes[i], status);
  }
}

void output_patient_stats(int patient_ids[], int num_patients, int avg_age) {
  for (int i = 0; i < num_patients; i++) {
    printf("Patient %d\n", patient_ids[i]);
  }
  printf("Average patient age: %d\n", avg_age);
}
