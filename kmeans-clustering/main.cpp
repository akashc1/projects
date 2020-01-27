/**
 * Akash Chaurasia (achaura1)
 * achaura1@jhu.edu
 * 601.220 (02)
 * Homework 7
 * November 8, 2017
 */

#include "dataset.h"
#include "kmeans.h"

int main(int argc, char *argv[]) {

  //make sure user makes correct input
  if (argc < 4) {
    cout << "Not enough arguments!" << endl;
  } else {
    const char* input_filename = argv[2];
    const char* output_filename = argv[3];

    Dataset d = Dataset(input_filename);
    if (d.size() > 0) {
      
      int num_clusters = atoi(argv[1]);

      KMeans k = KMeans(num_clusters, d.num_features());
    
      k.fit(d, 1);
      
    //    k.print_centroids();
    //
      k.write_clustered_data(d, output_filename);
    } else {
      cout << "Encountered error with opening \"" << input_filename << "\"!" << endl;
    }
  }
  return 0;
}
