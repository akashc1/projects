// this is a header guard
// we'll talk more about this later
#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * User interface for the patient
 * database with functions for the
 * exact prompts and output formatting
 * that you'll need for the assignment
 *
 * DO NOT MODIFY THE CODE IN THIS FILE
 * Feel free to add comments if you wish
 */

#include <stdio.h>
#include <stdlib.h>


/**
 * Print all menu options to the user
 */
void print_menu();

/**
 * Formatted output for when user enters n
 */
void output_n(int num_patients);

/**
 * Prompt for additional info when user enters d
 */
void prompt_d();


/**
 * Prompt for additional info when user enters e
 */
void prompt_e();

/**
 * Prompt for additional info when user enters c
 */
void prompt_c();

/**
 * Prompt for additional info when user enters l
 */
void prompt_l();

/**
 * Prompt for additional info when user enters s
 */
void prompt_s();

/**
 * Prompt for duplicate entry in the database
 */
void duplicate_entry(int id);

/**
 * Let the user know the supplied patient ID
 * does not exist
 */
void bad_patient_ID(int id);

/**
 * Let the user know the supplied disease code
 * does not exist
 */
void bad_disease_code(char *code);

/**
 * Bad menu option
 */
void bad_menu_option(char option);

/**
 * Formatted output for any prompts that require
 * patient disease profile output (prompts d, c)
 */
void output_disease_profile(int disease_profile[]);

/**
 * Formatted output for patient IDs
 */
void output_patient_stats(int patient_ids[], int num_patients, int avg_age);


#endif
// end of header guard
