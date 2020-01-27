/**
 * Akash Chaurasia (achaura1)
 * achaura1@jhu.edu
 * 601.220 (02)
 * Homework 7
 * November 9, 2017
 */

#include "kmeans.h"

/**
 * Creates a new array of centroids with coordinates of all
 * centroids INITIALIZED to 0.
 * Note this should create and return an array of proper dimensions
 * i.e. num_clusters * num_features
 * @return double** an array of centroids
 */
double** KMeans::create_centroids() const {

  //allocate memory for array
  double** new_dub = new double*[_num_clusters];
  for (int i = 0; i < _num_clusters; i++) {
    new_dub[i] = new double[_num_features];
    //iterate through array and set all values to 0.0
    for (int j = 0; j < _num_features; j++) {
      new_dub[i][j] = 0.0;
    }
  }
  return new_dub;
}


/**
 * Deletes a given array of centroids passed by reference
 * @param centroids a reference to an array of centroids
 */

void KMeans::delete_centroids(double**& centroids) {
  //delete allocated memory
  for (int i = 0; i < _num_clusters; i++) {
    delete[] centroids[i];
  }
  delete[] centroids;
}



/**
 * Performs the major iterative step of k-means.
 * Takes in a reference to the current map of cluster labels
 * to samples, and calculates the new centroids based on it.
 * It also decides whether to keep update the current centroids
 * based on a test with the inertia of the new clustering against
 * the minimum inertia that's been encountered
 * @param labels reference to a mapping from cluster label to a vector of
 * points that are assigned that label
 * @param db reference to a Dataset object of all points in labels
 */
void KMeans::update_centroids(Clusters& labels, const Dataset &db) {

   //create new centroid array
  double **new_centroids = create_centroids();
  for (int i = 0; i < _num_clusters; i++) {
    for (int j = 0; j < _num_features; j++) {
      double feature_sum = 0.0;

      //calculate sum of feature values for each cluster
      for (unsigned int k = 0; k < labels[i].size(); k++) {
	feature_sum += (labels[i])[k][j];
      }

      //set average if not an empty cluster
      if (labels[i].size() != 0) {
	new_centroids[i][j] = (feature_sum / labels[i].size());
      } else {
	//if empty cluster, set average to -10000 (from Piazza)
	new_centroids[i][j] = -10000;
      }
    }
  }

  //update labels to fit current centroids
  labels = predict(db);

  //calculate new inertia
  double new_inertia = calculate_inertia(new_centroids, labels);
  //if better inertia, make thse centroids the best centroids
  //and set new inertia to min_inertia
  if (new_inertia < _min_inertia) {
    _min_inertia = new_inertia;
    delete_centroids(cluster_centroids);
    cluster_centroids = new_centroids;
  } else {
    //if not an improvement, delete recently allocated centroids
    delete_centroids(new_centroids);
  }
  
}

/**
 * Calculate the Euclidean distance between two points/samples.                                                                                                                                                  
 * Remember Euclidean distance is the square root of the sum of the
 * squared distances between EACH coordinate of the two points.
 * @param point1 array of doubles corresponding to the first point
 * @param point2 array of doubles corresponding to the second point
 * @return double the calculated Euclidean distance
 */

double KMeans::calculate_distance(const double* point1, const double* point2) const {

  //calculate sum of squares of differences between corresponding data values
  double sum_to_square = 0.0;
  for (int i = 0; i < _num_features; i++) {
    double diff = (point1[i] - point2[i]);
    sum_to_square += (diff * diff);
  }

  //return square root of sum
  return (sqrt(sum_to_square));
}


/**
 * Calculate inertia as the sum of the within cluster distances.
 * @param centroids array of centroids
 * @param labels mapping of labels to points that fit the label
 * @return double the calcuated inertia
 */

double KMeans::calculate_inertia(double** centroids, Clusters& labels) const {

  double total_inertia = 0.0;

  //iterate through centroids & clusters and add up distances between points
  //and their cluster centroid
  for (int i = 0; i < _num_clusters; i++) {
    for (unsigned int j = 0; j < labels[i].size(); j++) {
      total_inertia += calculate_distance(centroids[i], (labels[i])[j]);
    }
  }

  return total_inertia;
}


/**
 * Non-default constructor for KMeans. Should
 * initialize ALL instance variables based on the arguments
 * provided.
 * @param num_clusters the k for k-means
 * @param num_features the number of features used to represent
 * each sample in the Dataset
 */
//initializer list used to intiialize number values
KMeans::KMeans(const int num_clusters, const int num_features) : _num_clusters(num_clusters), _num_features(num_features),
								 _min_inertia(1000000000) {
  //use already made method to initialize centroid array
  cluster_centroids = create_centroids();

}


/**
 * Destructor, perform any required cleanup over here
 * to enure there are no memory leaks
 */
KMeans::~KMeans() {
  //use method to deallocate array
  delete_centroids(cluster_centroids);
}


/**                                                                                                                          
 * Return the label of the centroid that is closest to the given
 * point (as given by Euclidean distance)                                                            
 * @param point array of doubles corresponding to point of interest
 * @return int the label of the closest cluster/centroid       
 */

int KMeans::find_closest_cluster(const double* point) const {

  //find inertia between point and first cluster centroid
  double pairwise_inertia = calculate_distance(point, cluster_centroids[0]);
  int closest_cluster = 0;
  //iterate through cluster centroids and calculate distances
  for (int i = 1; i < _num_clusters; i++) {
    double new_inertia = calculate_distance(point, cluster_centroids[i]);
    //if better cluster is found, set best cluster to that label
    if (new_inertia < pairwise_inertia) {
      pairwise_inertia = new_inertia;
      closest_cluster = i;
    }
  }

  return closest_cluster;  
}

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
 * @param data an object of Dataset, passed by reference
 * @param seed used to seed random number generator for initial guess of centroids
 */


void KMeans::fit(const Dataset& data, int seed) {

  //seed RNG and make Clusters object
  srand(seed);
  Clusters best_clusters;

  //iterate through cluster centroids and set them to
  //k random points from the given Dataset
  for (int i = 0; i < _num_clusters; i++) {
    int data_point_index = (rand() % data.size());

    for (int j = 0; j < _num_features; j++) {
      cluster_centroids[i][j] = data.get_data(data_point_index, j);
    }
  }
  
  //make first fit of clusters
  best_clusters = predict(data);
  
  //make new clusters and update centroids 300 times
  for (int count = 0; count < MAX_ITERATIONS; count++) {
    //call update_centroids to update the centroids each iteration
    update_centroids(best_clusters, data);
  }
}

/**
 * Return a mapping of cluster labels to points in the Dataset that fit that
 * label. See the typedef at the top for exactly what that looks like.
 * @param data an object of Dataset, passed by reference
 * @return Clusters the cluster mapping
 */

Clusters KMeans::predict(const Dataset& data) const {

  //make new Clusters object
  Clusters new_clusters;

  //iterate through all data points in Dataset
  for (int i = 0; i < data.size(); i++) {
    //get label of closest cluster for each data point
    int best_cluster = find_closest_cluster(data.get_point(i));
    //add point to chosen cluster
    new_clusters[best_cluster].push_back(data.get_point(i));
  }
  return new_clusters;
}

/**
 * Return the number of clusters
 * @return int representing number of clusters
 */

int KMeans::num_clusters() const {
  return _num_clusters;
}

/**
 * Print all centroids, with each coordinate of a given centroid TAB separated
 * and each centroid on a separate line
 */

void KMeans::print_centroids() const {
  for (int i = 0; i < _num_clusters; i++) {
    for (int j = 0; j < _num_features; j++) {
      cout << cluster_centroids[i][j] << "\t";
    }
    cout << endl;
  }
  
}


/**
 * Write the clustered data in a format that is exactly the same as the provided
 * reference file, i.e. TAB separated coordinates for each point and another TAB
 * followed by the cluster label. This should be one per line for each point
 * in the Dataset and written to a file given by filename
 * @param data object of Dataset, passed by reference
 * @param filename reference to a C-style string
 */                                                                                                                                                                                                              


void KMeans::write_clustered_data(const Dataset& data, const char*& filename) {

  //open file
  ofstream outfile(filename);
  if (outfile.is_open()) {
    //iterate through Datset points
    for (int i = 0; i < data.size(); i++) {                                                                                                                                                            
      for (int j = 0; j < _num_features; j++) {
	//write all data points as give
	outfile << data.get_data(i, j) << "\t";	
      }
      
      //write cluster this point belongs in
      outfile << find_closest_cluster(data.get_point(i)) << endl;
    }
    
    //close file
    outfile.close();
  } else {
    cout << "Unable to open " << filename << " for output!" << endl;
  }
}



