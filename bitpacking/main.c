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
#include "fileUtil.h"
#include "choices.h"

/**
   Main function. Takes user input direclty from 
   the command line used to call main function

   @param argc the number of arguments taken from the function call
   @param *argv an array of strings representing the arguments taken
   @return a number since main is an int
*/

int main(int argc, char *argv[]) {

  // open file pointer and get number of patients in file
  FILE *db = NULL;
  int num_patients = 0;
  if (argc > 1) { // make sure the argument we're about to read exists                                                                                                                                             
    db = fopen(argv[1], "r");
    if (!db) {
      printf("error: failed to open file \"%s\"\n", argv[1]);
    } else {
      fscanf(db, "%d", &num_patients);
    }
  }

  // create patient array and load
  unsigned int patients[num_patients];
  // using the output of a function to give the number of patients loaded
  num_patients = read_file(db, patients);
  // close file!!
  fclose(db);

  // print menu for first time and take first user input
  print_menu();
  char user_input;
  char full_user_input[255];
  fgets(full_user_input, 255, stdin);
  user_input = full_user_input[0];

  // loop that runs until user indicates he/she wishes to exit
  while(user_input != 'q') {
    switch (user_input) {
    case 'n' :
      choice_n(num_patients);
      break;

    case 'd' :
      choice_d(patients, num_patients);
      break;

    case 'c' :
      choice_c(patients, num_patients);
      break;

    case 'l' :
      choice_l(patients, num_patients);
      break;

    case 'e' :
      choice_e(patients, num_patients);
      break;

    case 's' :
      choice_s(patients, num_patients);
      break;

    default:
      bad_menu_option(user_input);

    }
    // print menu and get new user choice
    print_menu();
    fgets(full_user_input, 255, stdin);
    user_input = full_user_input[0];
  }
  // return 0 since this function is an int
  return 0;
}
