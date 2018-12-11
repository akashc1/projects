/*
Akash Chaurasia & Parth Vora
achaura1; pvora4
achaura1@jhu.edu; pvora4@jhu.edu
CS.220 Midterm project 1
16/10/2017
*/

#ifndef IMAGEIO_H
#define IMAGEIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pixels.h"

int writePPM(Pixel *image, int rows, int cols, int colors, FILE *fp);

int writePPMImage(Image *im, FILE *fp);

Image* readPPMImage(FILE *fp);

#endif
