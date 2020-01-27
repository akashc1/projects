/**
 * Akash Chaurasia (achaura1)
 * achaura1@jhu.edu
 * 601.220 (02)
 * Homework 7
 * November 8, 2017
 */

#include "dataset.h"
#include "kmeans.h"
#include <iomanip>

class Test {

 private:
  int test_pass;
  
 public:

  //all neames self explanatory, each one tests a function
  //from the Dataset or KMeans classes
  
  void test_kmeans_create_centroids();

  void test_kmeans_update_centroids();

  void test_kmeans_calculate_distance();

  void test_kmeans_calculate_inertia();

  int get_test_pass() { return test_pass;};

  void test_kmeans_constructor();

  void test_kmeans_find_closest_cluster();

  void test_kmeans_fit();

  void test_kmeans_predict();
};


//a function that compares two files
int file_compare(const string filename_a, const string filename_b);

void test_dataset_constructor();

void test_dataset_size();

void test_dataset_features();

void test_dataset_get_data();

void test_dataset_get_point();

void test_kmeans_clusters();
