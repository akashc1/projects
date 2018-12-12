/**
   Akash Chaurasia
   601.220 (02)
   Homework 6
   November 1, 2017
*/

#ifndef DBLOAD_H
#define DBLOAD_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>


using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::getline;
using std::vector;                                                                                                                                                                                               
using std::map;

//typedef map of string to int to "dictionary" to make easier to type
typedef map<string, int> dictionary;


//function that returns a dictionary map from a file given a filename
dictionary db_to_map(const string filename);

#endif
