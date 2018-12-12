/**
   Akash Chaurasia (achaura1)
   achaura1@jhu.edu
   601.220 (02)
   Homework 6
   November 1, 2017
*/

#include "select.h"

/**
   Function to go through all strings in a string vector and
   add them to a map of int to string if it exists in a given
   dictionary database map

   @param all_edits the string vector of edits to check
   @param db the string to int map dictionary database to check against
   @param suggestions the int to string map to add all verified strings to

*/

void known(const s_vec all_edits, const dictionary db, i_smap& suggestions) {

  //iterate through string vector
  for (unsigned int i = 0; i < all_edits.size(); i++) {

    //copy string in vector index
    string to_comp = all_edits[i];

    //try to find string in dictionary database map
    dictionary::const_iterator it = db.find(to_comp);

    //if found in dictionary database, add to int to string map
    if (it != db.cend()) {
      int weight = it->second;
      suggestions[weight] = to_comp;
    }
  }
}


/**
   Function to give a string to output that suggests
   a correction given a user input word. Returns "no correction found"
   if a correct string would require more than 2 edits

   @param user_input the input to correct
   @param db the dictionary database to compare edits to
   @return corrected the string to report to the user

*/

string correct(const string user_input, const dictionary db) {
  
  //create output string
  string corrected;

  //check if user input is a word in dictionary database
  dictionary::const_iterator it = db.find(user_input);
  if (it !=  db.cend()) {

    //if original input is in dictionary database, return it formatted for output
    corrected = "Did you mean " + user_input + "?";
    return corrected;
  } else { //string needs to be edited to be correct
    //create string vector of all edits, and add first iteration of edits
    s_vec all_edits;
    edits(user_input, all_edits);

    //create map of suggestions (edits in dictionary database)
    i_smap suggestions;
    //add all edits present in dictionary database to suggestion map
    known(all_edits, db, suggestions);

    //make iterator point to last element of integer to string map
    //which would have the largest weight
    auto it = suggestions.crbegin();

    //if none of the first iteration edits are present in the dictionary database
    if (suggestions.size() == 0) {

      //find size of edits vector
      unsigned int k = all_edits.size();

      //iterate through all existing edits in string vector
      for (unsigned int i = 0; i < k; i++) {
	//for every pre-existing edit, create second iteration edits and add to vector
	edits(all_edits[i], all_edits);                                                                                                                                                                          
      }

      //create suggestions map
      i_smap new_suggestions;
      //add all edits present in dictionary database to suggestions map
      known(all_edits, db, new_suggestions);

      //if no first or second iteration edits are present in dictionary database
      //then report no corrections found
      if (new_suggestions.size() == 0) {

	corrected = "No correction found";
	return corrected;
      } else { //one or more second iteration edits are present in the database
	//return word from second edits that has the largest weight
	corrected = "Did you mean " + (new_suggestions.crbegin())->second + "?";
	return corrected;
      }

    } else { //one or more first iteration edits are present in the dictionary database
      //return first edit that is in the dictionary database that has the largest weight
      corrected = "Did you mean " + it->second + "?";
    }                                                                                                                                                                                                            
  }

  //return formatted string to be output
  return corrected;
}
