/**
   Akash Chaurasia (achaura1)
   achaura1@jhu.edu
   601.220 (02)
   Homework 6
   November 1, 2017
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include "select.h"
#include "dbload.h"
#include <cassert>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::getline;
using std::vector;
using std::map;
using std::ofstream;


int file_compare(const string filename_a, const string filename_b);

void write__insertion_a(void);

void test_insertion_a();

void test_insertion_a();

void write_insertion_apeple(void);

void write_deletion_a(void);

void test_deletion_a();

void write_deletion_apeple(void);

void test_deletion_apeple();

void write_transposition_a(void);

void test_transposition_a();

void write_transposition_apeple(void);

void test_transposition_apeple();

void write_substitution_a(void);

void test_substitution_a();

void write_substitution_apeple(void);

void test_substitution_apeple();

void write_edits_a(void);

void test_edits_a();

void write_edits_apeple(void);

void test_edits_apeple();

void test_known();

void test_correct();
