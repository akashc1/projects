/**
 * Akash Chaurasia (achaura1)
 * achaura1@jhu.edu
 * 601.220 (02)
 * Homework 7
 * November 8, 2017
 */


#include "test.h"

/**
 * Function that compares two files
 * @param filename_a name of the first file
 * @param filename_b name of the second file
 * @return int 1 if files are equal, 0 otherwise
 * also prints all unequal lines
 */

int file_compare(const string filename_a, const string filename_b) {
  ifstream testfile_a(filename_a);
  ifstream testfile_b(filename_b);

  string line1, line2;
  int flag = 0;

  if (testfile_a.is_open() && testfile_b.is_open()) {
    flag = 1;
  }
  
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


/**
 * Method that tests the Dataset constructor by 
 * loading a data file into a new Dataset, 
 * printing the data to a file in identical format,
 * and comparing the two files.
 */
void test_dataset_constructor() {

  const char* data_file = "data/iris.txt";
  Dataset d = Dataset(data_file);
  d.print_data_to_file("iris_data.txt");

  int test = file_compare("data/iris.txt", "iris_data.txt");
  if (test) {
    cout << "Dataset's constructor works!" << endl;
  } else {
    cout << "Dataset constructor doesn't work!" << endl;
  }
}


/**
 * Tests the Dataset class's size() method
 * by making a new Dataset from "iris.txt"
 * and making sure the size function returns
 * 150 (the number of points in "iris.txt"
 */
void test_dataset_size() {

  const char* data_file = "data/iris.txt";
  Dataset d = Dataset(data_file);
  if (d.size() == 150) {
    cout << "Dataset's size() works!" << endl;
  } else {
    cout << "Dataset's size() doesnot work!" << endl;
  }
}

/**
 * Tests the Dataset class's features() method
 * by loading a dataset from "iris.txt" and making sure
 * the method returns 3 (the number of features in "iris.txt"
 */
void test_dataset_features() {
  const char* data_file = "data/iris.txt";
  Dataset d = Dataset(data_file);

  if (d.num_features() == 3) {
    cout << "Dataset's num_features() works!" << endl;
  } else {
    cout << "Dataset's num_features() doesn't work!" << endl;
  }
}


/**
 * Tests the Dataset class's get_data() method
 * by making a Datset from "iris.txt" and making sure
 * the method returns 0.2 (the 0, 0th value in iris.txt)
 */
void test_dataset_get_data() {
  const char* data_file = "data/iris.txt";
  Dataset d = Dataset(data_file);

  if (d.get_data(0, 0) == 0.2) {
    cout << "Dataset's get_data works!" << endl;
  } else {
    cout << "Dataset's get_data doesn't work!" << endl;
  }
}


/**
 * Tests the Dataset class's get_point method
 * by using it to extract the first point from its data
 * and making sure they are equal element by element
 */
void test_dataset_get_point() {
  const char* data_file = "data/iris.txt";
  Dataset d = Dataset(data_file);

  double *point = d.get_point(0);
  int flag = 1;
  for (int i = 0; i < d.num_features(); i++) {
    if (point[i] != d.get_data(0, i)) {
      flag = 0;
    }
  }

  if (flag) {
    cout << "Dataset's get_point works!" << endl;
  } else {
    cout << "Dataset's get_point doesn't work!" << endl;
  }
}


/**
 * Tests the KMeans class's create_centroids method
 * by making sure that it returns a double double array
 * of the correct size with all elements initialized to 0.0.
 */
void Test::test_kmeans_create_centroids() {

  KMeans k(3, 3);
  double** test_centroids = k.create_centroids();
  int flag = 1;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (test_centroids[i][j] != 0.0) {
	flag = 0;
      }
    }
  }
  if (flag) {
    cout << "KMeans's create_centroids works!" << endl;
  } else {
    cout << "KMeans's create_centroids doesn't work!" << endl;
  }
  k.delete_centroids(test_centroids);
}

/**
 * Tests the KMeans class's update_centroids method
 * by making clusters suiting the initial centroids,
 * calling update_centroids, and making sure that
 * the minmun inertia is lower than the previous one
 * and that the new centroids area all equal to the given cluster points
 */
void Test::test_kmeans_update_centroids() {
  KMeans k(3,3);

  const char* data_file = "data/iris.txt";
  Dataset d = Dataset(data_file);


  Clusters fit_clusters = k.predict(d);


  k.update_centroids(fit_clusters, d);

  int flag = 1;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (k.cluster_centroids[i][j] == 10) {
	flag = 0;
      }
    }
  }
  if ((k._min_inertia < 1000000000) && flag) {
    cout << "KMeans's update_centroids works!" << endl;
  } else {
    cout << "KMeans's update_centroids doesn't work!" << endl;
  }

}

/**
 * Tests the KMeans class's calculate_distance method
 * by making two points with a set distance
 * and making sure the method returns the correct distance
 */
void Test::test_kmeans_calculate_distance() {
  KMeans k(3,3);
  double *test_point1 = new double[3];
  double *test_point2 = new double[3];
  for (int i = 0; i < 3; i++) {
    test_point1[i] = 10;
    test_point2[i] = 0;
  }

  if (k.calculate_distance(test_point1, test_point2) == sqrt(300)) {
    cout << "KMeans's calculate_distance works!" << endl;
  } else {
    cout << "KMeans's calculate_distance doesn't work!" << endl;
  }
  delete[] test_point1;
  delete[] test_point2;
}


/**
 * Tests the KMeans class's calculate_inertia method
 * by making clusters and a KMeans object (whose 
 * centroids are all initialized to 0.0) and making sure
 * that the method returns the correct inertia
 */
void Test::test_kmeans_calculate_inertia() {
  KMeans k(3,3);
  double *test_point = new double[3];
  for (int i = 0; i < 3; i++) {
    test_point[i] = 10.0;
  }
  Clusters test_clusters;

  for (int i = 0; i < 3; i++) {
    test_clusters[i].push_back(test_point);
  }

  if (k.calculate_inertia(k.cluster_centroids, test_clusters) == (3 * sqrt(300))) {
    cout << "KMeans's calculate_inertia works!" << endl;
  } else {
    cout << "KMeans's calculate_inertia doesn't work!" << endl;
  }
  delete[] test_point;
}


/**
 * Tests the KMeans class's constructor by
 * making a new KMeans object and making sure the
 * instance variables have the desired initial values
 */
void Test::test_kmeans_constructor() {

  KMeans k(3,3);
  if ((k._num_features == 3) && (k._min_inertia == 1000000000) && (k.cluster_centroids[0][0] == 0.0)) {
    cout << "KMeans's constructor works!" << endl;
  } else {
    cout << "KMeans's constructor doesn't work!" << endl;
  }
}

/**
 * Tests the KMeans class's find_closest_cluster method
 * by making a point and a KMeans object, setting the 
 * KMeans object's centroids to set values, and making sure
 * that the method returns the correct cluster label
 */
void Test::test_kmeans_find_closest_cluster() {

  KMeans k(3,1);
  k.cluster_centroids[0][0] = 10;
  k.cluster_centroids[1][0] = 5;

  double *dub = new double[1];
  dub[0] = 10;

  int best_cluster = k.find_closest_cluster(dub);
  if (best_cluster == 0) {
    cout << "KMeans's find_closest_cluster works!" << endl;
  } else {
    cout << "KMeans's find_closest_cluster doesn't work!" << endl;
  }

  delete[] dub;
}


/**
 * Tests the KMeans class's fit method by
 * doing what the main method would do with "iris.txt"
 * and testing it against a given output file
 */
void Test::test_kmeans_fit() {

  const char* data_file = "data/iris.txt";
  Dataset d = Dataset(data_file);
  KMeans k(3, 3);

  const char* output_file = "test_fit_output.txt";
  k.fit(d, 1);
  k.write_clustered_data(d, output_file); 

  int test = file_compare(output_file, "tests/iris_clusters_3.txt");
  
  if (test) {
    cout << "KMeans's fit() works!" << endl;
  } else {
    cout << "KMeans's fit() doesn't work!" << endl;
  }
}

/**
 * Tests the KMeans class's predict method
 * by making KMeans and Dataset objects, using fit()
 * to fit the centroids to the data, then making a new
 * Clusters object and making sure that its inertia with 
 * the centroids matches the KMeans object's minimum inertia
 */
void Test::test_kmeans_predict() {

  const char* data_file = "data/iris.txt";
  Dataset d = Dataset(data_file);

  KMeans k(3, 3);

  k.fit(d, 1);

  Clusters best_clusters = k.predict(d);

  double current_inertia = k.calculate_inertia(k.cluster_centroids, best_clusters);

  if (current_inertia == k._min_inertia) {
    cout << "KMeans's predict() works!" << endl;
  } else {
    cout << "KMeans's predict() doesn't work!" << endl;
  }
}

/**
 * Tests the KMeans class's clusters method
 * by making a new KMeans object and making sure
 * that it initializes with the correct number of clusters
 */
void test_kmeans_clusters() {

  KMeans k(3,3);

  int num_clusters = k.num_clusters();

  if (num_clusters == 3) {
    cout << "KMeans's num_clusters works!" << endl;
  } else {
    cout << "KMeans's num_clusters doesn't work!" << endl;
  }
}
  
  
int main() {
  test_dataset_constructor();
  test_dataset_size();
  test_dataset_features();
  test_dataset_get_data();
  test_dataset_get_point();
  Test t;
  t.test_kmeans_create_centroids();
  t.test_kmeans_update_centroids();
  t.test_kmeans_calculate_distance();
  t.test_kmeans_calculate_inertia();
  t.test_kmeans_constructor();
  t.test_kmeans_find_closest_cluster();
  t.test_kmeans_fit();
  t.test_kmeans_predict();
  test_kmeans_clusters();
  return 0;
}


  
