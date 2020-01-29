# Image Segmentation --- Markov Random Fields, Expectation Maximization

I wanted to experiment with graphical models and Markov Random Fields (MRF) seemed very interesting. I found a [paper describing EM for segmentation](https://hal.inria.fr/inria-00072132/document) as well as the Potts model. This model is used more for statistical mechanics but seemed useful for segmentation as well.

#### Generating sample data

I used an RNG and added noise and a cosine function to generate data that looks like this:

![sample-data](code/output_4_0.png)


#### Playing with max depth

A max depth of 3 had the following sort of underfitting:

![depth-3](code/output_6_0.png)

And a max depth of 20 had pretty strong overfitting:

![depth-20](code/output_6_4.png)


### GBRT - Results of boosting

I had never implemented any sort of boosting algorithm so this was very interesting to play around with, especially considering that almost all of the learners (regression trees) are essentially fitting residual error.

#### Playing with number of boosting iterations

Similar to the depth in the regular regression tree, the GBRT with only 2 boosting iterations showed strong underfitting:

![gbrt-iter-2](code/output_8_0.png)

And with 100 iterations, the overfitting was very strong:

![gbrt-iter-2](code/output_8_4.png)

#### Regularization in regression trees

A regularization parameter of 0.6 resulted in a pretty nice fit of the generated data:

![gbrt-reg](code/output_10_2.png)
