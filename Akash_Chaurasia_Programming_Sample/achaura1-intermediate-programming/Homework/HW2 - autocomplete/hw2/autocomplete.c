/**
   Akash Chaurasia
   achaura1@jhu.edu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// define a max query length
#define MAX_QUERY_LENGTH 255

/**
   Function to load data file consisting
   of words and their frequencies

   @param queries array of strings of max length 255
   @param weights array of unsigned longs
   @param fp pointer to a data file
 */
void load(char queries[][MAX_QUERY_LENGTH], unsigned long weights[],
	  FILE *fp) {

  char query[MAX_QUERY_LENGTH];  // buffer for queries
  unsigned long weight;  // used to read in weights
  int i = 0;  // index

  // keep scanning until we fail to scan a query
  // weight
  while (fscanf(fp, "%lu", &weight) == 1) {

    // scan the space
    fgetc(fp);
    // scan the query
    fscanf(fp, "%[^\n]\n", query);

    // put in the appropriate query, weight pair
    strcpy(queries[i], query);
    weights[i] = weight;
    i++;
  }
}


/**
   Function to write queries and weights into a data file

   @param queries array of strings of max length 255
   @param weights array of unsigned longs
   @param fp pointer to a data file
   @param length number of elements in each array
 */
void write(char queries[][MAX_QUERY_LENGTH], unsigned long weights[],
	   FILE *fp, int length) {

  for (int i = 0; i < length; i++) {
    fprintf(fp, "%s\t%lu\n", queries[i], weights[i]);
  }
}

/**
   Function to swap two elements each of the queries and weights arrays
   using the same two indices

   @param queries array of strings of max length 255
   @param weights array ofunsigned longs
   @param x int representing index of first elements to be swapped
   @param y int representing index of second elements to be swapped
*/

void double_swap(char queries[][MAX_QUERY_LENGTH], unsigned long weights[],
		 int x, int y) {
  
  // create temp string for queries, same for weights
  char q_temp[MAX_QUERY_LENGTH];
  unsigned long w_temp = weights[x];

  // swap items in x, y using temp arrays/long
  strcpy(q_temp, queries[x]);
  strcpy(queries[x], queries[y]);
  strcpy(queries[y], q_temp);
  weights[x] = weights[y];
  weights[y] = w_temp;
}

/**
   Function to bubble sort queries according to alphabetical order

   @param queries array of strings of max length 255
   @param weights array of unsigned longs
   @param size int representing size of both arrays
*/
void ins_sort(char queries[][MAX_QUERY_LENGTH], unsigned long weights[],
	      int size) {
  // declare vars to be used
  int i, j, str_cmp;

  // outer loop of search
  for (i = 0; i < size; i++) {
    for (j = 0; j < size - 1; j++) {

      // define value for comparison of adjacent entries in queries
      str_cmp = strcmp(queries[j], queries[j + 1]);

      // swap entries at j and j + 1 if j + 1 entry should come first
      if (str_cmp > 0) {
	double_swap(queries, weights, j, j + 1);
      }
    }
  }
}

/**
   Function to bubble sort queries in order of decreasing weight
   
   @param queries array of strings of max length 255
   @param weights array of unsigned longs
   @param size int representing size of both arrays
*/
void w_sort(char queries[][MAX_QUERY_LENGTH], unsigned long weights[], int size) {

  // initiate variables
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size - 1; j++) {

      // swap elements of j and j + 1 if j + 1 has a larger weight
      if (weights[j] < weights[j + 1]) {
	double_swap(queries, weights, j, j + 1);
      }
    }
  }
}

/**
   Function to find the index associated with the user in put prompt

   @param queries array of strings of max length 255
   @param prompt string of user input to search for
   @param size int of size of array to search
   @return index of matching query
*/

int search(char queries[][MAX_QUERY_LENGTH], char prompt[], int size) {

  // initiate variables
  int left = 0;
  int mid;
  int prompt_size = strlen(prompt);

  //to exit when left is to the right of size
  while (left <= size) {

    // define middle index
    mid = (left + size)/ 2;

    // return mid if the entry at mid matches the prompt
    if (strncmp(prompt, queries[mid], prompt_size) == 0) {
      return mid;
    }

    // if prompt is to the right of mid, redefine left
    else if (strncmp(prompt, queries[mid], prompt_size) > 0) {
      left = mid + 1;
    }

    // reached if prompt is to the left of mid, redefine size
    else {
      size = mid - 1;
    }
    
  }
  // prompt not found, return -1
  return -1;
}

/**
   Function to find all matches in query array that match user prompt

   @param queries array of strings of max length 255
   @param matches array of strings that match the prompt
   @param weights array of unsigned longs
   @param m_weights array of weights of matched queries
   @param size int representing size of array to search through
   @param prompt string of user prompt to search for
   @param index starting index of first match to prompt in queries array
   @return total number of entries in matches array
*/

int match_search(char queries[][MAX_QUERY_LENGTH], char matches[][MAX_QUERY_LENGTH],
		  unsigned long weights[], unsigned long m_weights[], int size,
		  char prompt[], int index) {

  // define variables to be used
  int p_len = strlen(prompt);
  int i = index;
  int j = 0;

  // while prompt matches, move left without leaving array
  while ((i >= 0) && (strncmp(queries[i], prompt, p_len) == 0)) {
    if (strncmp(queries[i], prompt, p_len) == 0) {

      // copy entries and increment indices approprately
      strcpy(matches[j], queries[i]);
      m_weights[j] = weights[i];
      j++;
    }
    i--;
  }

  // start one to the right of the index since index was already scanned
  i = index + 1;

  // while prompt matches, move right without leaving array
  while ((i < size) && (strncmp(queries[i], prompt, p_len) == 0)) {
    if (strncmp(queries[i], prompt, p_len) == 0) {

      // copy entries and increment indices appropriately
      strcpy(matches[j], queries[i]);
      m_weights[j] = weights[i];
      j++;
    }
    i++;
  }

  // return the number of matches found in array
  return j;
}

/**
   Function to print matched queries with weights in correct format

   @param queries array of strings of max length 255
   @param weights array of unsigned long
   @param num_matches int of the number of matches found
 */

void print_matches(char queries[][MAX_QUERY_LENGTH], unsigned long weights[],
		   int num_matches) {

  // print queries and weights in format of query: weight
  for (int i = 0; (i < num_matches) && (i < 5); i++) {
    printf("%s: %lu\n", queries[i], weights[i]);
  }
}
  
/**
   Main function
 */
int main() {

  // name of the data file  
  char *data_file = "data/wiktionary.txt";
  char *sorted_file = "data/sorted_wiktionary.txt";
  // open a file pointer
  FILE *fp = fopen(data_file, "r");

  // scan number of queries in the vocabulary file
  int num_queries;
  fscanf(fp, "%d", &num_queries);

  // declare the parallel arrays
  char queries[num_queries][MAX_QUERY_LENGTH];
  unsigned long weights[num_queries];

  // read the data into the arrays
  load(queries, weights, fp);

  // alphabetically sort arrays
  ins_sort(queries, weights, num_queries);

  // always remember to close file pointers!
  fclose(fp);

  // write the sorted arrays to a file
  fp = fopen(sorted_file, "w");
  write(queries, weights, fp, num_queries);
  fclose(fp);

  // jump down a line to indicate completion of sorting
  printf("\n");
  char prompt[MAX_QUERY_LENGTH];

  // continue executing until EOF is reached
  while ((scanf("%s", prompt) == 1)) {

    // create parallel arrays for matches and their weights
    char matches[num_queries / 5][MAX_QUERY_LENGTH];
    unsigned long m_weights[num_queries / 5];

    // find index associated with prompt in sorted array
    int index = search(queries, prompt, num_queries);
  
    // compile all matches to prompt in array, assign number of matches to int
    int num_matches = match_search(queries, matches,
				   weights, m_weights,
				   num_queries, prompt, index);

    // sort both arrays in order of decreasing weight
    w_sort(matches, m_weights, num_matches);

    // report matches with weights in correct format
    print_matches(matches, m_weights, num_matches);
      
  }

  return 0;
}
