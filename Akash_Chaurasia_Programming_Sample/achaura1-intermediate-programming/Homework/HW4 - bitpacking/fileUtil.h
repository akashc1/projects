// this is a header guard                                                                                                                                                                                          
// we'll talk more about this later                                                                                                                                                                                
#ifndef FILEUTIL_H
#define FILEUTIL_H

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

/**                                                                                                                                                                                                                
 * Print all menu options to the user                                                                                                                                                                              
 */
int int_dis_status(char t1[3]);

/**                                                                                                                                                                                                                
 * Formatted output for when user enters n                                                                                                                                                                         
 */
void load_diseases(unsigned int patients[], char t1[], char t2[], char t3[],
		   char t4[], char t5[], int index);


int load_data(FILE *fp, unsigned int patients[]);

int read_file(FILE *fp, unsigned int patients[]);
#endif
