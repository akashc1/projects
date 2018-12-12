/**
   Akash Chaurasia (achaura1)
   achaura1@jhu.edu
   601.220 (02)
   Homework 6
   November 1, 2017
*/


#include "test.h"
#include "edit.h"
#include "dbload.h"
#include "select.h"

//function that checks if two files are exactly the same
//returns 1 if they are equal, 0 if they are not
int file_compare(const string filename_a, const string filename_b) {
  ifstream testfile_a(filename_a);
  ifstream testfile_b(filename_b);
  string line1, line2;
  int flag = 1;
  while (getline(testfile_a, line1) && getline(testfile_b, line2)) {
    if (line1 != line2) {
      cout << line1 << "\tis not equal to:\t" << line2 <<endl;
      flag = 0;
    }
  }
  testfile_a.close();
  testfile_b.close();
  return flag;
}

//writes a text file of all insertion edits of "a"
void write_insertion_a(void) {

  ofstream outfile("test_insertions_a.txt");
  dictionary result = db_to_map("data/gutenberg.txt");

  s_vec all_edits;
  insertion("a", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}


//tests if my insertion functions produces edits identical to
//given insertion edits of "a"
void test_insertion_a() {
  write_insertion_a();
  int result = file_compare("test_insertions_a.txt", "tests/insertions_a.txt");
  if (result) {
    cout << "Insertions function passes test for a!" << endl;
  } else {
    cout << "Insertions function does not work for a!" << endl;
    assert(result);
  }
}

//writes a text file of all insertion edits of "apeple"
void write_insertion_apeple(void) {
  ofstream outfile("test_insertions_apeple.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  insertion("apeple", all_edits);
  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}

//tests if my insertion function produces edits identical to
//given insertion edits of "apeple"
void test_insertion_apeple() {
  write_insertion_apeple();
  int result = file_compare("test_insertions_apeple.txt", "tests/insertions_apeple.txt");
  if (result) {
    cout << "Insertions function passes test for apeple!" << endl;
  } else {
    cout << "Insertions function does not work for apeple!" << endl;
    assert(result);
  }
}


//writes a text file of all deletion edits of "a"
void write_deletion_a(void) {
  ofstream outfile("test_deletions_a.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  deletion("a", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}


//tests if my deletion function produces edits identical to
//give deletion edits of "a"
void test_deletion_a() {

  write_deletion_a();
  int result = file_compare("test_deletions_a.txt", "tests/deletions_a.txt");

  if (result) {
    cout << "Deletions function passes test for a!" << endl;
  } else {
    cout << "Deletions function does not work for a!" << endl;
    assert(result);
  }
}


//writes a text file of all deletion edits of "apeple"
void write_deletion_apeple(void) {
  ofstream outfile("test_deletions_apeple.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  deletion("apeple", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}

//tests if my deletion function produces edits identical to
//given deletion edits of "apeple
void test_deletion_apeple() {

  write_deletion_apeple();
  int result = file_compare("test_deletions_apeple.txt", "tests/deletions_apeple.txt");

  if (result) {
    cout << "Deletions function passes test for apeple!" << endl;
  } else {
    cout << "Deletions function does not work for apeple!" << endl;
    assert(result);
  }
}


//writes a text file of all transposition edits of "a"
void write_transposition_a(void) {
  ofstream outfile("test_transpositions_a.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  transposition("a", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}

//tests if my transposition function produces edits identical to
//given transposition edits of "a"
void test_transposition_a() {

  write_transposition_a();
  int result = file_compare("test_transpositions_a.txt", "tests/transpositions_a.txt");

  if (result) {
    cout << "Transpositions function passes test for a!" << endl;
  } else {
    cout << "Transpositions function does not work for a!" << endl;
    assert(result);
    
  }
}

//writes a text file of all transposition edits of "apeple"
void write_transposition_apeple(void) {
  ofstream outfile("test_transpositions_apeple.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  transposition("apeple", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}

//tests if my transposition function produces edits identical to
//given transposition edits of "apeple"
void test_transposition_apeple() {

  write_transposition_apeple();
  int result = file_compare("test_transpositions_apeple.txt", "tests/transpositions_apeple.txt");

  if (result) {
    cout << "Transpositions function passes test for apeple!" << endl;
  } else {
    cout << "Transpositions function does not work for apeple!" << endl;
    assert(result);
  }
}

//writes a text file of substitution edits of "a"
void write_substitution_a(void) {
  ofstream outfile("test_substitutions_a.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  substitution("a", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}

//tests if my substitution function produces edits identical to
//given substitution edits of "a"
void test_substitution_a() {

  write_substitution_a();
  int result = file_compare("test_substitutions_a.txt", "tests/substitutions_a.txt");

  if (result) {
    cout << "Substitutions function passes test for a!" << endl;
  } else {
    cout << "Substitutions function does not work for a!" << endl;
    assert(result);
  }
}


//writes a text file of substitution edits of "apeple"
void write_substitution_apeple(void) {
  ofstream outfile("test_substitutions_apeple.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  substitution("apeple", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}

//tests if my substitution function produces edits identical to
//given substitution edits of "apeple"
void test_substitution_apeple() {

  write_substitution_apeple();
  int result = file_compare("test_substitutions_apeple.txt", "tests/substitutions_apeple.txt");

  if (result) {
    cout << "Substitutions function passes test for apeple!" << endl;
  } else {
    cout << "Substitutions function does not work for apeple!" << endl;
    assert(result);
  }
}

//writes a text file of all edits of "a"
void write_edits_a(void) {
  ofstream outfile("test_edits_a.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  edits("a", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}

//tests if my edits function produces edits identical to
//given edits of "a"
void test_edits_a() {

  write_edits_a();
  int result = file_compare("test_edits_a.txt", "tests/edits_a.txt");

  if (result) {
    cout << "Edits function passes test for a!" << endl;
  } else {
    cout << "Edits function does not work for a!" << endl;
    assert(result);
  }
}

//writes a text file of edits of "apeple"
void write_edits_apeple(void) {
  ofstream outfile("test_edits_apeple.txt");
  dictionary result = db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  edits("apeple", all_edits);

  for (unsigned int i = 0; i < all_edits.size(); i++) {
    outfile << all_edits[i] << endl;
  }
  outfile.close();
}

//tests if my edits function produces edits identical to
//given edits of "apeple"
void test_edits_apeple() {

  write_edits_apeple();
  int result = file_compare("test_edits_apeple.txt", "tests/edits_apeple.txt");

  if (result) {
    cout << "Edits function passes test for apeple!" << endl;
  } else {
    cout << "Edits function does not work for apeple!" << endl;
    assert(result);
  }
}

//tests if my known function truly only adds strings from a string vector
//to an i_smap if they are present in a given dictionary database
void test_known() {
  
  dictionary db =  db_to_map("data/gutenberg.txt");
  s_vec all_edits;
  edits("apeple", all_edits);
  i_smap suggestions;
  known(all_edits, db, suggestions);
  int flag = 1;
  for (i_smap::const_iterator it = suggestions.cbegin();
       it != suggestions.cend(); it++) {
    string suggestion = it->second;
    if (db.find(suggestion) == db.end()) {
      flag = 0;
    }
  }
  if (flag) {
    cout << "Known function passes test!" << endl;
  } else {
    cout << "Known function does not work!" << endl;
    assert(flag);
  }
  
}


//tests if my correct function works as required for all cases
//using given words and an additional word
void test_correct() {
  dictionary test_db1;
  test_db1["testing"] = 10;

  string test_1 = correct("testing", test_db1);
  string test_2 = correct("testingg", test_db1);
  string test_3 = correct("testinngg", test_db1);

  int flag = 1;
  string test_1_res1 = "Did you mean testing?";
  string test_1_res2 = "No correction found";
  if ((test_1 != test_1_res1) || (test_2 != test_1_res1) || (test_3 != test_1_res1)) {
    flag = 0;
    cout << "Small test cases failed!" << endl;
  }

  string test_4 = correct("gibberish", test_db1);
  if (test_4 != test_1_res2) {
    flag = 0;
    cout << "Small test cases for no corrections found failed!" << endl;
  }
  
  dictionary test_db2 = db_to_map("data/gutenberg.txt");
  string test_5 = correct("testing", test_db2);
  string test_6 = correct("testingg", test_db2);
  string test_7 = correct("testinngg", test_db2);
  string test_8 = correct("gibberish", test_db2);

  if ((test_5 != test_1_res1) || (test_6 != test_1_res1) || (test_7 != test_1_res1)) {
    flag = 0;
    cout << "Test cases for \"testing\" failed!" << endl;
  }

  if (test_8 != test_1_res2) {
    flag = 0;
    cout << "Test cases for no corrections found failed!" << endl;
  }
		     
  string test2_res1 = "Did you mean a?";
  string test2_res2 = "No correction found";

  string test_9 = correct("a", test_db2);
  string test_10 = correct("qa", test_db2);
  string test_11 = correct("qqa", test_db2);
  string test_12 = correct("gibberish", test_db2);

  if ((test_9 != test2_res1) || (test_10 != test2_res1) || (test_11 != test2_res1)) {
    flag = 0;
    cout << "Test cases for \"a\" failed!" << endl;
  }

  if (test_12 != test2_res2) {
    flag = 0;
    cout << "Test cases for no courrections found failed!" << endl;
  }

  string test3_res1 = "Did you mean apple?";
  string test3_res2 = "No correction found";

  string test_13 = correct("apple", test_db2);
  string test_14 = correct("apeple", test_db2);
  string test_15 = correct("abpeple", test_db2);
  string test_16 = correct("gibberish", test_db2);

  if ((test_13 != test3_res1) || (test_14 != test3_res1) || (test_15 != test3_res1)) {
    flag = 0;
    cout << "Test cases for \"apple\" failed!" << endl;
  }

  if (test_16 != test3_res2) {
    flag = 0;
    cout << "Test cases for no courrections found failed!" << endl;
  }

  assert(flag);
  if (flag) {
    cout << "Correct function passes all tests!" << endl;
  }
}

  

int main() {

  //call test functions, report output
  test_insertion_a();
  test_insertion_apeple();
  test_deletion_a();
  test_deletion_apeple();
  test_transposition_a();
  test_transposition_apeple();
  test_substitution_a();
  test_substitution_apeple();
  test_edits_a();
  test_edits_apeple();
  test_known();
  test_correct();
  return 0;
}

