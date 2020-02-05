## Deep Learning-based Computer Vision

Many people believe that deep learning can solve vision entirely. I'm not sure if that is the case, but neural networks have solved a variety of difficult vision tasks in a variety of ways. I wanted to try some of them out. Original notebooks were run on [Google Colab](https://colab.research.google.com/) and can be found in `src`.

### [Part 1: Fashion MNIST](fashion_mnist.pdf)

Anybody who's used deep learning for vision has probably heard of [the MNIST dataset](http://yann.lecun.com/exdb/mnist/), which sets up the image classification task for handwritten digits. I wanted to do something a little different and found a [fashion dataset of similar format.](https://github.com/zalandoresearch/fashion-mnist)

I played around with a few different configurations of networks and modulating hyperparameters. 

##### Results

Configuration | Accuracy on Validation Set
------------- | --------------------------
Baseline model: batch size 64, epochs 3, learning rate 0.01, conv1 output 6 channels | 87.5%
Increased batch size (32) | 87.4%
More epochs (10) | 88.3%
Lower learning rate (0.001) | 85.9%
Increased conv1 output dimensionality (32 channels) | 90.1%
Add dropout layer after conv1 (p = 0.3) | 88.7%

**Combined network:** batch size 64, epochs 10, learning rate 0.01, conv1 output 32 channels, added dropout layer (p = 0.3)
**Final Accuracy on Test set: 88.0%**

##### Summary

These experiments made a lot of sense. Here were my general conclusions:
* Batch size isn't a huge determinant of the network's performance. I've generally seen 32 and 64 used as the most common batch sizes, and I'll probably stick with those unless I see convincing literature about it.
* Increasing the number of epochs increased the accuracy on the validation set. This could be unreliable, since it's likely very data dependent; drastically increasing epochs seems like a very easy way to overfit the training set.
* The lower learning rate likely kept the model from reaching a local minimum in the given number of epochs, or it may have found a different local minimum with worse accuracy.
* The increased channel dimensionality output from the first convolution layer really increased the model's accuracy. This is a pretty simple model, so perhaps it has too much bias and is underfitting the data, which the added feature space mitigates.
* Dropout increased accuracy, most likely by preventing co-adaptation of nodes in the model. 


### [Part 2: Transfer Learning](caltech_101.pdf)

A method I found rather interesting is [transfer learning](https://blogs.nvidia.com/blog/2019/02/07/what-is-transfer-learning/). I wanted to implement it using [MobileNetV2](https://arxiv.org/abs/1801.04381) on the [Caltech 101 Dataset](http://www.vision.caltech.edu/Image_Datasets/Caltech101/), which has 101 image classes. This ended up being a good PyTorch exercise.

##### Results

**Pre-trained network:** batch size 64, learning rate 0.005, 50 epochs  ** *Accuracy: 90.7% ** *

##### Summary
<img src="./label-circ-match/output/text.png" alt="text" width="200"/>

