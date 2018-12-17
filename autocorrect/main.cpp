/**
   Akash Chaurasia
   akashc@jhu.edu
*/

#include <iostream>
#include <cassert>
#include <string>
#include "dbload.h"
#include <map>
#include <string>
#include <vector>
#include "edit.h"
#include "select.h"

using std::map;
using std::string;
using std::cout;
using std::endl;
using std::cin;

int main() {

  string  name;
  cin >> name;
  dictionary result = db_to_map("data/gutenberg.txt");
  while (name != "quit") {
    

    string corrected = correct(name, result);
    cout << corrected <<endl;
    cin >> name;
  }
  return EXIT_SUCCESS;
}								       

