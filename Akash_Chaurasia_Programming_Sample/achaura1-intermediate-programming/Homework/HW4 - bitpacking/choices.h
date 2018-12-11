// this is a header guard                                                                                                                                                                                          
// we'll talk more about this later                                                                                                                                                                                
#ifndef CHOICES_H
#define CHOICES_H

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
#include "interface.h"
#include "fileUtil.h"
/**                                                                                                                                                                                                                
 * Print all menu options to the user                                                                                                                                                                              
 */
int find_id(unsigned int patients[], unsigned int id, int num_patients);

int check_bad_code(char dis_codes[][3], int num_diseases);
// to interpret the status of a disease (treated, etc.)                                                                                                                                                            
int interpret_dis(unsigned int status);

void array_swap_ints(unsigned int diseased_patients[], int x, int y);

void sort_ids(unsigned int diseased_patients[], int num_patients);

void choice_n(int num_patients);


  
void choice_d(unsigned int patients[], int num_patients);

void choice_c(unsigned int patients[], int num_patients);

void choice_l(unsigned int patients[], int num_patients);

void choice_e(unsigned int patients[], int num_patients);

void choice_s(unsigned int patients[], int num_patients);
#endif
