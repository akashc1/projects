/**
   Akash Chaurasia (achaura1)
   achaura1@jhu.edu
   601.220 (02)
   Homework 6
   November 1, 2017
*/

#include "edit.h"

/**
   Function to return the nth letter of the alphabet

   @param i the numbered alphabet to return
   @return a string with one desired alphabetical character

*/
string get_nth_letter(const int i) {

  //make string of alphabet to refer to
  string alphabet = "abcdefghijklmnopqrstuvwxyz";

  //make sure that index is appropriate
  if (0 <= i && i <= 25) {
    //return letter at index position
    return string(1, alphabet.at(i));
  } else { return NULL; }
}
    
      
      
/**
   Function to add all possible insertion edits to string vector
   given a word to make edits of

   @param user_string the string to make edits of
   @param all_edits the typedef'd vector of strings to add edits onto
*/

void insertion(const string user_string, s_vec& all_edits) {

  //find size of string
  int str_size = user_string.size();

  for(int i = 0; i < 26; i++) {
    for (int j = 0; j < str_size; j++) {

      //create pre-edited string identical to original string
      string new_edit = user_string;

      //insert character at every possible position except last
      //using get_nth_letter function which returns all letters
      //of the alphabet iteratively
      new_edit.insert(j, get_nth_letter(i));

      //add edited string to string vector
      all_edits.push_back(new_edit);
    }

    //can't use string.insert function to add to end of string
    //so use append after iterating through all other positions
    string new_edit = user_string;
    all_edits.push_back(new_edit.append(get_nth_letter(i)));
  }

}


/**
   Function to make all possible deletion edits of a given string
   and add them to a given string vector

   @param user_string the string on which to make edits
   @param all_edits the string vector to add all edits to

*/

void deletion(const string user_string, s_vec& all_edits) {

  //find size of given string
  int str_size = user_string.size();

  //iterate through all possible characters to edit
  for (int i = 0; i < str_size; i++) {

    //create pre-edited string identical to original string
    string new_edit = user_string;
    //erase character at ith position
    new_edit.erase(i, 1);
    //add new edit to string vector
    all_edits.push_back(new_edit);
  }

}


/**
   Function to create all possible transposition edits of a given string
   and add them to a given string vector

   @param user_string the string on which to make edits
   @param all_edits the string vector to add all edits to

*/

void transposition(const string user_string, s_vec& all_edits) {

  //find size of given string
  int str_size = user_string.size();

  //iterate through all first of two adjacent characters to swap
  //can't go to size index because loop uses (i + 1)th character
  
  for (int i = 0; i < str_size - 1; i++) {

    //create pre-edited string identical to original string
    string new_edit = user_string;

    //use swap function from algorithm library to swap adjacent characters
    swap(new_edit[i], new_edit[i + 1]);
    //add new edit to string vector
    all_edits.push_back(new_edit);
  }

}


/**
   Function to create all possible substitution edits of a given string
   and add them to a given string vector
   
   @param user_string the string on which to make all edits
   @param all_edits the string vector to add all edits to

*/

void substitution(const string user_string, s_vec& all_edits) {

  //find size of given string
  int str_size = user_string.size();

  //iterate through all letters
  for (int i = 0; i < 26; i++) {
    //iterate through all letters in string
    for (int j = 0; j < str_size; j++) {
      //create pre-edited string identical to original string
      string new_edit = user_string;

      //change original letter at index to letter obtained
      //using get_nth_letter function
      new_edit[j] = (get_nth_letter(i))[0];
      //add new edit to string vector
      all_edits.push_back(new_edit);
    }
  }

}


/**
   Function to make all possible edits of a given string
   and add them all to a given string vector

   @param user_string the string on which to make edits
   @param all_edits the string vector to add all edits to

*/

void edits(const string user_string, s_vec& all_edits) {

  //call all edit functions in specified order
  insertion(user_string, all_edits);
  deletion(user_string, all_edits);
  transposition(user_string, all_edits);
  substitution(user_string, all_edits);

}
  
