/**
   Akash Chaurasia (achaura1)
   achaura1@jhu.edu
   601.220 (02)
   Homework 6
   November 1, 2017
*/

#include "dbload.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

//declare functions used to make it easier to type
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::getline;
using std::vector;
using std::map;


/**
   Function to create a map of string to int (typedef'd to dictionary)
   using a given filename

   @param filename the string representation of the file name
   @return result the typedef'd map of string to int of words and frequencies
*/

dictionary db_to_map(const string filename) {

  //declare dictionary
  dictionary result;
  ifstream db_file(filename);
  string line;

  //loop to keep reading lines from file
  while(getline(db_file, line)) {

    string word;
    int weight;

    //create string stream from line obtained from file
    istringstream is(line);

    //parse into corresponding objects
    is >> word;
    is >> weight;
    //input into map
    result[word] = weight;
  }

  //close file
  db_file.close();    
  
  //return dictionary
  return result;

}

  

