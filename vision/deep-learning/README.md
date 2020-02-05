## Deep Learning-based Computer Vision

Many people believe that deep learning can solve vision entirely. I'm not sure if that is the case, but neural networks have solved a variety of difficult vision tasks in a variety of ways. I wanted to try some of them out. Original notebooks were run on [Google Colab](https://colab.research.google.com/) and can be found in `src`.

### Part 1: Fashion MNIST

Anybody who's used deep learning for vision has probably heard of [the MNIST dataset](http://yann.lecun.com/exdb/mnist/), which sets up the image classification task for handwritten digits. I wanted to do something a little different and found a fashion dataset of similar format.

I played around with a few different configurations of networks, playing around with hyperparameters. Here's a table summaraizing the results:

Configuration | Accuracy on Validation Set
------------- | --------------------------
Baseline model | 87.5%

<img src="./label-circ-match/output/text.png" alt="text" width="200"/>

![temp-match2](./label-circ-match/output/king.png)

#### [Image Alignment, RANSAC Feature Matching, and Stitching](./im-alignment)

This part came from my interest in image transformations and feature extraction. I implemented a Harris corner detector, a RANSAC algorithm to match corners from 2 images, and a least-squares method to solve for a transformation to align two given images. There's also a simple method of stitching the aligned images together.

<img src="./im-alignment/output/leuven_stacked.png" alt="align-stack" width="600"/>

<img src="./im-alignment/output/leuven_RANSACstacked.png" alt="align-ransac" width="600"/>

<img src="./im-alignment/output/leuven_stitched.png" alt="align-stitched" width="600"/>
