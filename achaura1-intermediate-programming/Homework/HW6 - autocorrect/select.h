/**
   Akash Chaurasia (achaura1)
   achaura1@jhu.edu
   601.220 (02)
   Homework 6
   November 1, 2017
*/
#ifndef SELECT_H
#define SELECT_H


#include "dbload.h"
#include "edit.h"

//typedef map of int to string to "i_smap" to facilitate typing
typedef map<int, string> i_smap;

//function to check all strings in a string vector for existence in dictionary map
//and add all verified edits to a suggestions i_smap
void known(const s_vec all_edits, const dictionary db, i_smap& suggestions);

//function to return the corrected word or report no corrections found
//given a string to correct and a dictionary database to compare against
string correct(const string user_input, const dictionary db);


#endif
