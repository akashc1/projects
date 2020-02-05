## Stereo Vision

A large component of traditional vision is stereovision. This area doesn't excite me too much, but I did want to try some things out for understanding.

#### Epipolar Geometry

Matches from two images can be used to determine a fundamental matrix. This essentially gives, given a match from one image, the line in the second image along which the corresponding match will be found.

All of these lines intersect at the epipole. I found matches from both images (in `data/matches.txt`) and used them to find the fundamental matrix, draw the epipolar lines which, if done correctly, all intersect at one point (epipole).

Here's the results:

![epi-results](./output/hopkins_epi.png)

