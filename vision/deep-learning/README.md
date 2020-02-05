## Deep Learning-based Computer Vision

Many people believe that deep learning can solve vision entirely. I'm not sure if that is the case, but neural networks have solved a variety of difficult vision tasks in a variety of ways. I wanted to try some of them out.

#### [Image Labeling, Hough Circle Detection, Template Matching](./label-circ-match)

This implementation of image labeling is pretty quick and can identify individual objects in an image. This is not a learning-based approach, rather a standard sequential labeling algorithm.

![labeling](./label-circ-match/output/coins_labeled.png)

The circle detection was just based on my interest in the Hough voting methods for circle/line detection, and the circle case seemed more interesting to me.

![circ-detect](./label-circ-match/output/coins_circles.png)

Template matching seemed very straightforward to me but as I expected, there were the edge cases I had to think about which was a quick and interesting challenge.

<img src="./label-circ-match/output/text.png" alt="text" width="200"/>

![temp-match2](./label-circ-match/output/king.png)

#### [Image Alignment, RANSAC Feature Matching, and Stitching](./im-alignment)

This part came from my interest in image transformations and feature extraction. I implemented a Harris corner detector, a RANSAC algorithm to match corners from 2 images, and a least-squares method to solve for a transformation to align two given images. There's also a simple method of stitching the aligned images together.

<img src="./im-alignment/output/leuven_stacked.png" alt="align-stack" width="600"/>

<img src="./im-alignment/output/leuven_RANSACstacked.png" alt="align-ransac" width="600"/>

<img src="./im-alignment/output/leuven_stitched.png" alt="align-stitched" width="600"/>
