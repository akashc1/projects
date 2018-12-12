/** 
 * Akash Chaurasia (achaura1)
 * achaura1@jhu.edu
 * 601.220 (02)
 * Homework 7
 * November 9, 2017
 */


#include "dataset.h"


/**
 * Constructor for a Datset object
 *
 * param: filename the name of the file to read data points from
 */

Dataset::Dataset(const char*& filename) {

  //open file to read from
  ifstream data_file(filename);
  if (data_file.is_open()) {

    //assign size and number of features
    data_file >> _size;
    data_file >> _num_features;

    //allocate memory for data array
    data = new double*[_size];
    for (int i = 0; i < _size; i++) {
      data[i] = new double[_num_features];
    }

    //read data from file into data array
    for (int i = 0; i < _size; i++) {
      for (int j = 0; j < _num_features; j++) {
	data_file >> data[i][j];
      }
    }
    //close data
    data_file.close();
  } else {
    //if file not found, initialize values to zero
    //this is in case file is not found, program will not crash
    cout << "Unable to open file!" << endl;
    _size = 0;
    _num_features = 0;
    data = new double*[1];
    data[0] = new double[1];
    data[0][0] = 0.0;
  }
  
}
    
/** 
 * Destructor, perform any required cleanup over here
 * to ensure there are no memory leaks   
 */
Dataset::~Dataset() {

  //free allocated memory
  for (int i = 0; i < _size; i++) {
    delete[] data[i];
  }
  delete[] data;
}

/**
 * Returns the size of the dataset
 * @return int corresponding to size of the dataset
 */
int Dataset::size() const {
  return _size;
}

/**
 * Returns the number of features used
 * to represent each sample
 * @return int representing number of features
 */  
int Dataset::num_features() const {
  return _num_features;
}

/**
 * Print the data to standard output in
 * the exact same format as it was read in
 * i.e. as a tab separated file with 2 lines
 * at the beginning indicating size and num_features
 */
void Dataset::print_data() const {

  //print size and num_features to stdout
  cout << _size << endl << _num_features << endl;

  //iterate through data array and print data points to stdout
  for (int i = 0; i < _size; i++) {
    for (int j = 0; j < _num_features; j++) {
      cout << data[i][j] << "\t";
    }
    cout << endl;
  }
}


/**
 * Get the data present at the i-th row and j-th
 * column
 * @param i integer for row
 * @param j integer for column
 * @return double representing data at the given coordinate  
 */
double Dataset::get_data(int i, int j) const {
  return data[i][j];
}

/**
 * Get the entire sample in the i-th row
 * @param i integer for row
 * @return double* representing array of doubles
 */
double* Dataset::get_point(int i) const {
  return data[i];
}

/**                                                                                                                                                                                                              
 * Print dataset to given file                                                                                                                                                                                   
 * @param filename name of file to print to
 */   
void Dataset::print_data_to_file(const string filename) {
  //open file to print to
  ofstream outfile(filename);
  //print to file if able to open file with given filename
  if (outfile.is_open()) {

    //print size and num_features
    outfile << _size << endl;
    outfile << _num_features << endl;

    //print data points with 2 decimal points (for testing purposes)
    for (int i = 0; i < _size; i++) {
      int j;
      for (j = 0; j < (_num_features - 1); j++) {
	outfile << std::fixed << std::setprecision(2) << data[i][j] << "\t";
      }
      outfile << std::fixed << std::setprecision(2) << data[i][j] << endl;

    }

    //close file
    outfile.close();
  } else {
    //if for some reason unable to open outfile
    cout << "Unable to open output file to print data to file!" << endl;
  }

}
      
