/*
Akash Chaurasia & Parth Vora
achaura1; pvora4
achaura1@jhu.edu; pvora4@jhu.edu
CS.220 Midterm project 1
16/10/2017
*/

#ifndef PIXELS_H
#define PIXELS_H

#include <stdlib.h>

//Pixel struct holds red, green, blue values of a pixel
typedef struct _pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;

//Image struct holds an array of pixels
typedef struct _image {
  Pixel *data;
  int rows;
  int cols;
  int colors;
} Image;

#endif
