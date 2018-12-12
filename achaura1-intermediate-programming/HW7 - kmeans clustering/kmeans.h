/**
 * Akash Chaurasia (achaura1)
 * achaura1@jhu.edu
 * 601.220 (02)
 * Homework 7
 * November 8, 2017
 */

#ifndef KMEANS_H
#define KMEANS_H

#include "dataset.h"
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>

using std::cout;
using std::endl;


// This is a convenient typedef to refer to
// a mapping that maps cluster labels (denoted
// as plain integers 0, 1, 2 etc.) to a vector
// of points/samples that have been assigned that label
typedef std::map<int, std::vector<double*>> Clusters;

/**
 * The KMeans class is used to perform clustering
 * on any given members of the Dataset class using
 * the k-means algorithm as described on the handout
 */
class KMeans {
  friend class Test;
 private:

  // Instance variables
  // ------------------

  // a 2D array corresponding to the centroids of
  // each cluster
  double** cluster_centroids;

  // the number of clusters
  int _num_clusters;

  // the number of features used to represent each sample
  // note this will be the same as the one used in the Dataset
  int _num_features;

  // the minimum inertia encountered so far
  // initially, you should initialize this to
  // some arbitrarily big number say 100000
  double _min_inertia;

  // Private functions
  // -----------------

  /**
   * Creates a new array of centroids with coordinates of all
   * centroids INITIALIZED to 0.

   * Note this should create and return an array of proper dimensions
   * i.e. num_clusters * num_features

   * @return
   * double** : an array of centroids
   */
  double** create_centroids() const;

  /**
   * Deletes a given array of centroids passed by reference

   * @param
   * centroids : a reference to an array of centroids
   */
  void delete_centroids(double**& centroids);

  /**
   * Performs the major iterative step of k-means.
   * Takes in a reference to the current map of cluster labels
   * to samples, and calculates the new centroids based on it.
   * It also decides whether to keep update the current centroids
   * based on a test with the inertia of the new clustering against
   * the minimum inertia that's been encountered
   * @param
   * labels : reference to a mapping from cluster label to a vector of
   *          points that are assigned that label
   */
  void update_centroids(Clusters& labels, const Dataset &db);

  /**
   * Calculate the Euclidean distance between two points/samples.
   * Remember Euclidean distance is the square root of the sum of the
   * squared distances between EACH coordinate of the two points.

   * @param
   * point1 : array of doubles corresponding to the first point
   * point2 : array of doubles corresponding to the second point

   * @return
   * double : the calculated Euclidean distance
   */
  double calculate_distance(const double* point1, const double* point2) const;

  /**
   * Calculate inertia as the sum of the within cluster distances.

   * @param
   * centroids : array of centroids
   * labels : mapping of labels to points that fit the label
   * @return
   * double : the calcuated inertia
   */
  double calculate_inertia(double** centroids, Clusters& labels) const;

 public:
  /**
   * Non-default constructor for KMeans. Should
   * initialize ALL instance variables based on the arguments
   * provided.

   * @param
   * num_clusters : the k for k-means
   * num_features : the number of features used to represent 
                    each sample in the Dataset
   */
  KMeans(const int num_clusters, const int num_features);

  /**
   * Destructor, perform any required cleanup over here
   * to enure there are no memory leaks
   */
  ~KMeans();

  int find_closest_cluster(const double* point) const;

  
  /**
   * Fits clusters i.e. generates suitable centroids that cluster the
   * given Dataset appropriately. 
   * Centroids are initialized here with points within the Dataset
   * itself. These points are picked by using a random number generator
   * that produces the indices of num_cluster number of points
   * The generator is seeded so it always gives the same points.
   * The algorithm continues for MAX_ITERATIONS and at
   * each iteration computes the predicted clusters given the current
   * centroids, calculates new centroids based on the predicted
   * clusters, and chooses to keep or discard the new centroids based
   * on the inertia metric.
   
   * @param
   * data : an object of Dataset, passed by reference
   * seed : used to seed random number generator for initial guess of centroids
   */
  
  void fit(const Dataset& data, int seed);

  /**
   * Return a mapping of cluster labels to points in the Dataset that fit that
   * label. See the typedef at the top for exactly what that looks like.
   * 
   * @param data an object of Dataset, passed by reference
   * @return Clusters the cluster mapping
   */
  Clusters predict(const Dataset& data) const;

  /**
   * Return the number of clusters
   *
   * @return int number of clusters
   */
  int num_clusters() const;

  /**
   * Print all centroids, with each coordinate of a given centroid TAB separated
   * and each centroid on a separate line
   */
  void print_centroids() const;

  /**
   * Write the clustered data in a format that is exactly the same as the provided
   * reference file, i.e. TAB separated coordinates for each point and another TAB
   * followed by the cluster label. This should be one per line for each point
   * in the Dataset and written to a file given by filename
   * 
   * @param data object of Dataset, passed by reference
   * filename : reference to a C-style string
   */
  void write_clustered_data(const Dataset& data, const char*& filename);
};




#endif
