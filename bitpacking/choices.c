/** 
    Akash Chaurasia
    akashc@jhu.edu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "choices.h"

/**
   A function to find the index of a given patient ID in a given array.

   @param patients the array of unsigned integers representing the patients
   @param id the id that the function is to search for
   @param num_patients the number of patients in the array
   @return int representing the index of the requested patient ID
*/

int find_id(unsigned int patients[], unsigned int id, int num_patients) {
  int i;

  // search through array, use bit shifting to compare IDs
  for (i = 0; i < num_patients; i++) {
    unsigned int sample_id = (patients[i] >> 17);
    if (sample_id == id) {
      return i;
    }
  }
  // return -1 if not found
  return -1;
}

/**
   A function to check if any of the disease codes entered are not 
   for one of the five given diseases
   
   @param dis_codes an array of strings representing the input disease codes
   @apram num_diseases the number of user input disease codes in the array
   @return int representing the index of a bad code, or -1 if there are no bad codes
*/

int check_bad_code(char dis_codes[][3], int num_diseases) {
  
  for (int i = 0; i < num_diseases; i++) {
    // see if the disease code in each element of the array matches one of the 5 disease codes
    int dis1 = strcmp("HU", dis_codes[i]);
    int dis2 = strcmp("FH", dis_codes[i]);
    int dis3 = strcmp("SC", dis_codes[i]);
    int dis4 = strcmp("TS", dis_codes[i]);
    int dis5 = strcmp("CF", dis_codes[i]);

    if (!((dis1 == 0) || (dis2 == 0) || (dis3 == 0) || (dis4 == 0) || (dis5 == 0))) {
      // return index of bad disease code
      return i;
    }
  }
  // no bad disease code, return -1
  return -1;
}
  
/**
   A function to use bit masking to see what a patient's disease status
   of a given disease is

   @param status the given patient's bit representation
   @return the numerical representation of the patient's disease status
*/
int interpret_dis(unsigned int status) {
  // using bit masking
  int result = (status & 3);
  return result;
}


/**
   A function to swap two elements of an array of patients

   @param diseased_patients the array of patients positive for given diseases
   @param x the index of one of the two elements to swap
   @param y the index of the second of the two elements to swap
*/

void array_swap_ints(unsigned int diseased_patients[], int x, int y) {
  // create temp and swap the two elements of the array
  unsigned int temp = diseased_patients[x];
  diseased_patients[x] = diseased_patients[y];
  diseased_patients[y] = temp;
}


/**
   A function to bubble sort the patients with certain diseases
   according to patient ID in increasing order

   @param diseased_patients an array representing the patients's IDs
   @param num_patients the number of patients in the array
*/

void sort_ids(unsigned int diseased_patients[], int num_patients) {
  // outer loop for bubble sorting
  for (int i = 0; i < num_patients - 1; i++) {
    // inner loop for bubble sorting
    for (int j = 0; j < num_patients - i - 1; j++) {
      // swap elements if lower element is smaller
      if (diseased_patients[j] > diseased_patients[j + 1]) {
	array_swap_ints(diseased_patients, j, j + 1);
      }
    }
  }

}


/**
   A function to print out how many patients are in the array
   using the given function from the interface file
   @param num_patients the number of patients in the array
*/

void choice_n(int num_patients) {
  output_n(num_patients);
}


/**
   A function to perform the tasks required of choice d, which is
   to report the disease status of a patient using a user input patient
   ID

   @param patients the array of patients
   @param num_patients the number of patients in the array
*/

void choice_d(unsigned int patients[], int num_patients) {
  // prompt user input, receive it, and parse it to an unsigned int
  prompt_d();
  char input[1024];
  fgets(input, 1024, stdin);
  unsigned int id;
  sscanf(input, "%u\n", &id);

  // if patient is found, load statuses for diseases in array and report
  int id_index = find_id(patients, id, num_patients);
  int disease_statuses[5];
  if (id_index >= 0) {
    unsigned int proxy = patients[id_index];
    for (int i = 4; i >= 0; i--) {
      disease_statuses[i] = interpret_dis(proxy);
      proxy = proxy >> 2;
    }
    
    output_disease_profile(disease_statuses);

    // if patient is not found, report bad ID
  } else {
    bad_patient_ID((int) id);
  }
  
}

/**
   A function to perform the tasks of choice c, which is to 
   take the user input of two patient IDs, and report the 
   disease statuses of potential offspring

   @param patients the array of patients
   @param num_patients the number of patients in the array
*/
void choice_c(unsigned int patients[], int num_patients) {
  // prompt user input and assign it to usable variables
  prompt_c();
  char input[1024];
  fgets(input, 1024, stdin);
  unsigned int id1, id2;
  int id1_index, id2_index;
  if (sscanf(input, "%u %u\n", &id1, &id2) == 2) {
    id1_index = find_id(patients, id1, num_patients);
    id2_index = find_id(patients, id2, num_patients);

    // proceed if both patients are found in the array
    if ((id1_index >= 0) && (id2_index >= 0)) {
      int id1_disease_statuses[5], id2_disease_statuses[5];
      unsigned int proxy1 = patients[id1_index];
      unsigned int proxy2 = patients[id2_index];

      // load two patient disease statuses in parallel arrays
      for (int i = 4; i >= 0; i--) {
	id1_disease_statuses[i] = interpret_dis(proxy1);
	id2_disease_statuses[i] = interpret_dis(proxy2);
	proxy1 = proxy1 >> 2;
	proxy2 = proxy2 >> 2;
      }

      // find and load potential child disease statuses in array
      int child_disease_statuses[5];
      // for dominant diseases (HU, FH)
      for (int i = 0; i < 2; i++) {
	if ((id1_disease_statuses[i] > 1) || (id2_disease_statuses[i] > 1)) {
	  child_disease_statuses[i] = 2;
	} else if ((id1_disease_statuses[i] == 1) && (id2_disease_statuses[i] == 1)) {
	  child_disease_statuses[i] = 1;
	} else {
	  child_disease_statuses[i] = 0;
	}
      }
      // for recessive diseases (SC, TS, CF)
      for (int i = 2; i < 5; i++) {
	if ((id1_disease_statuses[i] > 1) && (id2_disease_statuses[i] > 1)) {
	  child_disease_statuses[i] = 2;
	} else if ((id1_disease_statuses[i] == 0) || (id2_disease_statuses[i] == 0)) {
	  child_disease_statuses[i] = 0;
	} else {
	  child_disease_statuses[i] = 1;
	}
      }

      // report offspring disease status
      output_disease_profile(child_disease_statuses);

      // report bad user input if applicable
    } else {
      if (id1_index < 0) {
	bad_patient_ID((int) id1);
      }
      if (id2_index < 0) {
	bad_patient_ID((int) id2);
      }

  
    }
  } else {
    printf("Incorrect input\n");
  }
}


/**
    A function to perform the tasks of choice l, which is
    to print out all the patients IDs and average age of all
    patients that are positive for diseases indicated by the user

    @param patients the array of patients
    @param num_patients the number of patients in the array
*/

void choice_l(unsigned int patients[], int num_patients) {

  // prompt and assign user input to string
  prompt_l();
  char user_input[17];
  fgets(user_input, 1024, stdin);

  char dis_codes[5][3];
  int num_entries;


  // assign disease codes in string array
  num_entries = sscanf(user_input, "%s %s %s %s %s", dis_codes[0], dis_codes[1], dis_codes[2],
			   dis_codes[3], dis_codes[4]);

  int desired_diseases[5];
  for (int i = 0; i < 5; i++) {
    desired_diseases[i] = 0;
  }

  // check for mathes for select diseases, indicate in array
  for (int i = 0; i < num_entries; i++) {
    if (strcmp(dis_codes[i], "HU") == 0) {
      desired_diseases[0] = 1;
    }
  }

  for (int i = 0; i < num_entries; i++) {
    if (strcmp(dis_codes[i], "FH") == 0) {
      desired_diseases[1] = 1;
    }
  }

  for (int i = 0; i < num_entries; i++) {
    if (strcmp(dis_codes[i], "SC") == 0) {
      desired_diseases[2] = 1;
    }
  }

  for (int i = 0; i < num_entries; i++) {
    if (strcmp(dis_codes[i], "TS") == 0) {
      desired_diseases[3] = 1;
    }
  }

  for (int i = 0; i < num_entries; i++) {
    if (strcmp(dis_codes[i], "CF") == 0) {
      desired_diseases[4] = 1;
    }
  }

  // report bad disease code if applicable
  int is_bad_code = check_bad_code(dis_codes, num_entries);
  if (is_bad_code >= 0) {
    bad_disease_code(dis_codes[is_bad_code]);
  } else {
    // create array of patients positive for selected diseases
    unsigned int diseased_patients[num_patients];
    int j = 0;
    for (int i = 0; i < num_patients; i++) {
      int flag = 1;
      for (int k = 0; k < 5; k++) {
	if (desired_diseases[k]) {
	  unsigned int proxy = (patients[i] >> (8 - (2 * k)));
	  if (interpret_dis(proxy) < 2) {
	    flag = 0;
	  }
	}

      }
      if (flag) {
	diseased_patients[j] = patients[i];
	j++;
      }

    }

    // calculate average age and sort based on patient IDs
    int avg_age = 0;
    for (int i = 0; i < j; i++) {
      unsigned int age = ((diseased_patients[i] >> 10) & 127);
      avg_age += age;
      diseased_patients[i] = diseased_patients[i] >> 17;
    }
    avg_age = avg_age / j;
    sort_ids(diseased_patients, j);
    int patient_ids[j];
    for (int i = 0; i < j; i++) {
      patient_ids[i] = (int) diseased_patients[i];
    }
    // report patient stats
    output_patient_stats(patient_ids, j, avg_age);
  }
  
}


/**
   A function to perform the tasks of choices e, which is 
   to edit the disease status of a patient using the user input
   patient ID, the selected disease, and a new disease status

   @param patients the array of patients
   @param num_patients the number of patients in the array
*/

void choice_e(unsigned int patients[], int num_patients) {
  // prompt and assign user input
  prompt_e();
  char user_input[1024];
  fgets(user_input, 1024, stdin);

  unsigned int id;
  char dis_code[3];
  char new_dis_status[3];
  sscanf(user_input, "%u %s %s\n", &id, dis_code, new_dis_status);

  
  int id_index = find_id(patients, id, num_patients);
  // report bad ID if ID not found in array
  if (id_index < 0) {
    bad_patient_ID((int) id);
  } else {
    // identify the disease entered
    int disease_code = -1;
    if (strcmp("HU", dis_code) == 0) {
      disease_code = 8;
    }

    if (strcmp("FH", dis_code) == 0) {
      disease_code = 6;
    }

    if (strcmp("SC", dis_code) == 0) {
      disease_code = 4;
    }

    if (strcmp("TS", dis_code) == 0) {
      disease_code = 2;
    }

    if (strcmp("CF", dis_code) == 0) {
      disease_code = 0;
    }
    // report if disease code is not one of the 5 given ones
    if (disease_code < 0) {
      bad_disease_code(dis_code);
    } else {
      // get new disease code based on user input
      int new_disease_code = int_dis_status(new_dis_status);
     
      unsigned int current_patient = patients[id_index] >> disease_code;
      int current_disease_code = interpret_dis(current_patient);
      // change patient's disease code if it needs to be changed
      if (current_disease_code != new_disease_code) {
	patients[id_index] -= current_disease_code << disease_code;
	patients[id_index] += new_disease_code << disease_code;
      }
    }
  }
}


/**
   A function to perform the tasks of choice s, which
   is to save the database in binary representation 
   as a file named according to the user input

   @param patients the array of patients
   @param num_patients the number of patients in the array
*/

void choice_s(unsigned int patients[], int num_patients) {

  // prompt and assign user input
  prompt_s();
  char output_filename[255];

  fgets(output_filename, 255, stdin);
  
  char outfile_name[255];
  sscanf(output_filename, "%s\n", outfile_name);

  // open file pointer
  FILE *outfile = fopen(outfile_name, "w");

  // print out individual bits using bit masking and shifting
  for (int i = 0; i < num_patients; i++) {
    for (int j = 31; j >= 0; j--) {
      unsigned int bit = (patients[i] >> j) & 1;
      fprintf(outfile, "%u", bit);
    }
    fprintf(outfile, "\n");
  }

  // close file!
  fclose(outfile);
}
