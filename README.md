# Projects

## Akash Chaurasia
### akashc@jhu.edu
A collection of projects I have worked on or am currently working on. I mostly work in Python and C++, with experience in C, Java, and MATLAB. My preferred languages are C++ and Python.

I am an undergraduate student at [Johns Hopkins University](https://www.jhu.edu/) studying Biomedical Engineering and Computer Science. I am interested in medical technology, specifically the application of machine learning and deep learning methods in healthcare. 

##### [Data Science](https://github.com/akashc1/projects/tree/master/data-science)
Applying data science methods in Python on human biomedical data including ECG waveforms, MRI images, and action potential waveforms. Includes some machine learning techniques and frameworks such as [scikit-learn](https://scikit-learn.org/stable/) and others, with more to come!

##### [Learning Models](https://github.com/akashc1/projects/tree/master/learning-models)
A large set of machine learning models I have implemented, from a simple multiclass perceptron to Expectation Maximization on graphical models. This covers some vision (segmentation), a good amount of natural language processing, and various other types of models.

##### [Vision](https://github.com/akashc1/projects/tree/master/vision)
Some vision tasks I have taken on. Most of it is lower level stuff I implemented to get an understanding of how stuff works. Most of these things are probably in OpenCV.

##### [Autocomplete](https://github.com/akashc1/projects/tree/master/autocomplete)
A straightforward program in C that completes an input word. Uses a space-delmited text file with words and their respective weights (number of occurences in books in the [Project Gutenberg library](https://www.gutenberg.org/)), and recommends a completed word based on this weight. 

##### [Autocorrect](https://github.com/akashc1/projects/tree/master/autocorrect)
C++ program that corrects a given input. Uses a space-delmited text file with words with their respective weights. It calculates various edits of the input word (insertions, deletions, substitutions, transpositions) to find real words and makes a recommendation based on weight.

##### [Bitpacking](https://github.com/akashc1/projects/tree/master/bitpacking)
A C program that uses bitpacking to store information on patients and their disease conditions for 5 diseases. Quite rudimentary but used to learn binary options in C. Can defintely be expanded for other applications.

##### [Chess](https://github.com/akashc1/projects/tree/master/chess-cpp)
A large C++ project to implement a game of chess from the command line. I've impemented a couple versions on top of the classic chess implementation, including Spooky Chess, in which there is a ghost piece with random movement that captures pieces in spaces it lands on (except the King!), and King fo the Hill, in which getting your King to the middle 4 spots on the board is a victory. Beware of bugs!

##### [Image Processing](https://github.com/akashc1/projects/tree/master/image-processing)
A project in C that uses [PPM images](https://people.cs.clemson.edu/~yfeaste/cpsc101/CPSC101F15Yvon/Lectures/Oct1-ppm/PPM_Images.pdf) for various image processing techniques, including swapping color channels, changing brightness, cropping, converting to grayscale, changing contrast, [Gaussian blur](https://en.wikipedia.org/wiki/Gaussian_blur), sharpening, and edge detection. Can also write the images to a PPM file.

##### [*k*-means Clustering](https://github.com/akashc1/projects/tree/master/kmeans-clustering)
A project in C++ that performs [*k*-means clustering](https://en.wikipedia.org/wiki/K-means_clustering) on a dataset from a file (space-delimited, of course). User inputs the number of clusters to use, and the program prints to a file the data points and the cluster number it belongs to.
