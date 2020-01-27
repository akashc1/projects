/**
   Akash Chaurasia (achaura1)
   akashc@jhu.edu
*/

#ifndef EDIT_H
#define EDIT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <utility>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::getline;
using std::vector;
using std::map;
using std::swap;
typedef vector<string> s_vec;

//function that returns the ith letter of the alphabet
string get_nth_letter(const int i);

//function that makes all insertion edits of a string and
//adds them to a given string vector
void insertion(string user_string, s_vec& all_edits);

//function that makes all deletion edits of a string and
//adds them to a given string vector
void deletion(const string user_string, s_vec& all_edits);

//function that makes all transposition edits of a string and
//adds them to a given string vector
void transposition(const string user_string, s_vec& all_edits);

//function that makes all substitution edits of a string and
//adds them to a given string vector
void substitution(const string user_string, s_vec& all_edits);

//function that makes all possible edits of all kinds of a string
//and adds them to a given string vector
void edits(const string user_string, s_vec& all_edits);


#endif                 
