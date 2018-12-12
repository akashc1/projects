/**
 * Akash Chaurasia (achaura1)
 * achaura1@jhu.edu
 * 601.220 (02)
 * Homework 7
 * November 9, 2017
 */

#ifndef DATASET_H
#define DATASET_H

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
//define MAX_ITERATIONS to avoid using a magic number in KMeans class
#define MAX_ITERATIONS 300
//declare functions used to make it easier to type
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::string;
using std::getline;
using std::cin;


/**
 * The Dataset class is used to load and store
 * data, and provides an interface to securely
 * access its contents in downstream functions
 */
class Dataset {
 private:

  // data is a 2D dynamically allocated
  // array where each row corresponds
  // to a sample from our data
  double **data;

  // the number of rows in data i.e. number of samples
  int _size;

  // the number of columns in data i.e. number of dimensions
  // of our feature space
  int _num_features;

 public:

  /**
   * Non-default constructor for data that takes
   * in a filename as a C-style string (to use with 
   * const char *argv[]). The data, _size, and
   * _num_features should be initialized based on
   * the contents of the given file
   * @param filename reference to a C-style string 
   */
  Dataset(const char*& filename);

  /**
   * Destructor, perform any required cleanup over here
   * to ensure there are no memory leaks
   */
  ~Dataset();

  /**
   * Returns the size of the dataset
   * @return int corresponding to size of the dataset
   */
  int size() const;

  /**
   * Returns the number of features used 
   * to represent each sample
   * @return int number of features
   */
  int num_features() const;

  /**
   * Print the data to standard output in
   * the exact same format as it was read in
   * i.e. as a tab separated file with 2 lines
   * at the beginning indicating size and num_features
   */
  void print_data() const;

  /**
   * Get the data present at the i-th row and j-th
   * column
   * @param i integer for row
   * @param j integer for column
   * 
   * @return double data at the given coordinate
   */
  double get_data(int i, int j) const;

  /**
   * Get the entire sample in the i-th row
   * @param i integer for row
   * @return double* array of doubles
   */
  double* get_point(int i) const;

  /**
   * Print dataset to given file
   *
   * @param filename name of file to print to
   */
  void print_data_to_file(const string filename);
};

#endif
