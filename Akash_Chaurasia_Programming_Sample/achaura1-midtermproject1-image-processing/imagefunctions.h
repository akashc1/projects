/*
Akash Chaurasia & Parth Vora
achaura1; pvora4
achaura1@jhu.edu; pvora4@jhu.edu
CS.220 Midterm project 1
16/10/2017
*/

#ifndef IMAGEFUNCTIONS_H
#define IMAGEFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pixels.h"
#include "menu.h"

void colour_swap();

void change_brightness(Image *origImage, int brightness);

Image *crop(Image *origImage, int x1, int y1, int x2, int y2);

void greyscale(Image *origImage);

void change_contrast(Image *origImage, double contrast);

Pixel* blur(Image *origImage, double sigma);

Pixel* sharpen(double sigma, double amount, Image* origImage);

void detect_edges(Image *im, double sigma, double threshold);

unsigned char adjust_contrast(unsigned char colour, double contrast);

Pixel check_saturation(Pixel test, int brightness);

int* checkbounds(int row, int col, int size, Image *im);

double* garray(double sigma);

Pixel convolve(int row, int col, int *bounds, int size, double *gaussarray, Image *im);

#endif
