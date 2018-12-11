/*
Akash Chaurasia & Parth Vora
achaura1; pvora4
achaura1@jhu.edu; pvora4@jhu.edu
CS.220 Midterm project 1
Section 2
16/10/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "imagefunctions.h"
#include "pixels.h"
#include "imageIO.h"
#include <math.h>
#define PI 3.14159265

/**
   A Function that swaps the colour channels of an image

   @param Image *origImage the image to be modified
*/
void colour_swap(Image *origImage) {

  if(origImage == NULL) {
    error_null();
  } else {

    //For each pixel, swap the colours
    for (int i = 0; i < (origImage->rows * origImage->cols); i++) {
      Pixel tempPixel = origImage->data[i];
      origImage->data[i].r = tempPixel.g;
      origImage->data[i].g = tempPixel.b;
      origImage->data[i].b = tempPixel.r;
    }
  }
}

/**
   A function that changes the brightness of an image
   
   @param Image *origImage the image to be modified
   @param int brightness the level of brightness to be added or taken away
*/
void change_brightness(Image *origImage, int brightness) {

  if(origImage == NULL) {
    error_null();
  } else {
    
    //for each pixel, calls a function that changes brightness and checks saturation
    for(int i = 0; i < (origImage->rows * origImage -> cols); i++) {
      origImage->data[i] = check_saturation(origImage->data[i], brightness);  
    }
    
  }
}

/**
   A function that crops an image
   @param Image *origImage the image to be cropped
   @param int x1 the x coordinate of the first point
   @param int y1 the y coordinate of the first point
   @param int x2 the x coordinate of the second point
   @param int y2 the y coordinate of the second point
   @return An image that is the original image cropped to specifications
*/

Image *crop(Image *origImage, int x1, int y1, int x2, int y2) {
  if(origImage == NULL) {
    error_null();
    return NULL;
  }

  //Check that (x1, y1) and (x2, y2) are inbounds
  if(x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0) {
    printf("Out of bounds\n");
    return NULL;
  } else if (x1 > origImage->rows || x2 > origImage->rows || y1 > origImage->cols || y2 > origImage->cols) {
    printf("Out of bounds\n");
    return NULL;
    //Check if the points are degenerate -> a line or a point.
  } else if (x1 == x2 || y1 == y2) {
    printf("Degenerate box\n");
    return NULL;
  }
  
  //Figure out which point comes first
  //Swaps points if the second is before the first
  if(x1 > x2) {
    int temp = x1;
    x1 = x2;
    x2 = temp;
  }
  if(y1 > y2) {
    int temp = y1;
    y1 = y2;
    y2 = temp;
  }
  
  int new_rows = x2-x1+1; //new # of cols
  int new_cols = y2-y1+1; //new # of rows
  Pixel *temp = malloc(sizeof(Pixel) * new_rows * new_cols); //new pixel array for cropped image
  int count = 0; //index of temp pixel array
    
  //Take every pixel that is in bounds in the original image and put it into the temp array in order
  for(int i = x1; i <= x2; i++) { 
    for(int j = y1; j <= y2; j++) {
      temp[count] = origImage->data[i*(origImage->cols) + j];
      count ++;
    }
  }
    
  Image *result = malloc(sizeof(Image)); //new cropped image
  result->data = temp;
  result->rows = new_rows;
  result->cols = new_cols;
  result->colors = 255;
  
  return result; //return the cropped image
}

/**
   A function that changes an image to greyscale

   @param Image *origImage the image to be modified
*/
void greyscale(Image *origImage) {
  if(origImage == NULL) {
    error_null();
  } else {

    char intensity = 0;
    
    //calculates intensity for each pixel
    //reassigns the value of each colour channel to intensity
    for (int i = 0; i < (origImage->rows * origImage->cols); i++) {
      intensity = 0.30*(origImage->data[i].r) + 0.59*(origImage->data[i].g) + 0.11*(origImage->data[i].b);
      origImage->data[i].r = intensity;
      origImage->data[i].g = intensity;
      origImage->data[i].b = intensity;
    }
  }
}
  
/**
   A function that changes the contrast of an image
   @param Image *origImage the image to be modified
   @param double contrast the amount by which contrast is to be increased
*/
void change_contrast(Image *origImage, double contrast) {
  if(origImage == NULL) {
    error_null();
  } else if(contrast <= 0) {
    printf("ERROR: contrast must be positive\n");
  } else {
    
    //for each colour channel of each pixel, call adjust_contrast
    //adjust_contrast returns the adjusted value for each colour channel
    for (int i = 0; i < (origImage->rows * origImage->cols); i++) {
      origImage->data[i].r = adjust_contrast(origImage->data[i].r, contrast);
      origImage->data[i].g = adjust_contrast(origImage->data[i].g, contrast);
      origImage->data[i].b = adjust_contrast(origImage->data[i].b, contrast);
    }  
  }
}

/**
   A function that returns a blurs and image

   @param Image *origImage the image to be blurred
   @param double sigma the radius of the blur to be applied
   @return Pixel *temp the pixel array representing the blurred image
*/

Pixel* blur(Image *origImage, double sigma) {
  if(origImage == NULL) {
    error_null();
    return NULL;
  }

  if(sigma <= 0) {
    printf("ERROR: sigma must be positive\n");
    return NULL;
  }

    //first, initalize the gaussian array
    double *gaussarray = garray(sigma); 
    int size = sigma * 10;
    if(size % 2 == 0) {size += 1;} 
  
    //Now, we convolve the matrix for each pixel
    Pixel *temp = malloc(sizeof(Pixel) * origImage->rows * origImage->cols); //new pixel array
    for(int row = 0; row < origImage->rows; row++) {
      for(int col = 0; col < origImage->cols; col++) {
	//get the bounds for the array
	//check bounds returns an int array that tells you how far you can go in the left, right, up, down directions
	int *bounds = checkbounds(row, col, size, origImage);
      
	//convolve the gaussian array for the pixel with all elements in bounds
	temp[row*(origImage->cols) + col] = convolve(row, col, bounds, size, gaussarray, origImage);
	free(bounds); 
      }
    }
    free(gaussarray);
    return temp; //return the blurred pixel array
  
}

/**
   A function that sharpens an image
   @param double sigma the range to be applied for the sharpen
   @param double strength the strength of the sharpen to be applied
   @param Image *origImage the image to be sharpened
   @return Pixel *pixels the array of pixels representing the sharpened image
*/

Pixel* sharpen(double sigma, double strength, Image *origImage) {
  if(origImage == NULL) {
    error_null();
    return NULL;
  }

  if(sigma <= 0) {
    printf("ERROR: sigma must be positive\n");
    return NULL;
  }

  if(strength <= 0) {
    printf("ERROR: strength must be positive\n");
    return NULL;
  }

  //first, compute a blurred image
  Pixel *blurry = blur(origImage, sigma); 
  double diff_r= 0.0, diff_g = 0.0, diff_b = 0.0; //these will be the difference between the blurry and sharp images
  int red = 0, green = 0, blue= 0; //the new values for the colour channels

  Pixel *pixels = malloc(sizeof(Pixel) * origImage->rows * origImage->cols); //new pixel array 
  for(int i = 0; i < (origImage->rows * origImage->cols); i++) {

    //compute difference between blurred and normal image
    diff_r = (double)(origImage->data[i].r - blurry[i].r) * strength;
    diff_g = (double)(origImage->data[i].g - blurry[i].g) * strength;
    diff_b = (double)(origImage->data[i].b - blurry[i].b) * strength;
    red = origImage->data[i].r + (int)diff_r;
    green = origImage->data[i].g + (int)diff_g;
    blue = origImage->data[i].b + (int)diff_b;

    //check saturation
    if(red < 0) red = 0;
    if(red > 255) red = 255;
    if(green < 0) green = 0;
    if(green > 255) green = 255;
    if(blue < 0) blue = 0;
    if(blue > 255) blue = 255;

    //assign to a pixel
    pixels[i].r = (unsigned char)red;
    pixels[i].g = (unsigned char)green;
    pixels[i].b = (unsigned char)blue;
    
  }
  free(blurry);
  return pixels; //return the sharpened pixel array
}

/**
   A function that detects edges in an image using the Canny edge detector
   @param Image *im the image on which edge detect is to be applied
   @param double sigma the radius of the blur to be applied
   @param double threshold the minimum value used to determine an edge
*/
void detect_edges(Image *im, double sigma, double threshold) {
  if(im == NULL) {
    error_null();
  } else if(sigma <= 0) {
    printf("ERROR: sigma must be positive\n");
  } else if(threshold <= 0) {
    printf("ERROR: threshold must be positive\n");
  } else {

    Image *temp = malloc(sizeof(Image)); //temp image
    temp->data =  blur(im, sigma); //blur the image to reduce noise
    temp->rows = im->rows; //initializing to prevent memory problems
    temp->cols = im->cols;
    temp->colors = im->colors;
    greyscale(temp); //reduce image to intensity

    double grad_x = 0.0, grad_y = 0.0, grad_mag = 0.0; //these hold the gradients in x, y, and the magnitude of the gradient

    //for every pixel that is not an edge pixel
    for(int i = 1; i < im->rows - 1; i++) {
      for(int j = 1; j < im->cols - 1; j++) {

	//compute the x, y gradients and the magnitude of the gradient
	grad_x = (((double)temp->data[i*im->cols + (j+1)].r) - ((double)temp->data[i*im->cols + (j-1)].r))/2.0 ;
	grad_y = (((double)temp->data[(i+1)*im->cols + j].r) - ((double)temp->data[(i-1)*im->cols + j].r))/2.0 ;
	grad_mag = sqrt((grad_x * grad_x) + (grad_y * grad_y));

	//if the magnitude of the gradient is greater than the threshold, make the pixel black
	if(grad_mag > threshold) {
	  im->data[i*im->cols + j].r = 0;
	  im->data[i*im->cols + j].g = 0;
	  im->data[i*im->cols + j].b = 0;

	  //else make the pixel white
	} else {
	  im->data[i*im->cols + j].r = 255;
	  im->data[i*im->cols + j].g = 255;
	  im->data[i*im->cols + j].b = 255;
	}
      }
    }
    free(temp->data);
    free(temp);
  }
}
/**
   A function that checks for over/under saturation and adjusts contrast

   @param unsigned char colour the color of a given pixel color channel
   @param double contrast the amount by which contrast is to be changed
   @return unsigned char colour the new value of the color
*/

unsigned char adjust_contrast(unsigned char colour, double contrast) {

  //normalizes the colour on a scale from -1 to 1
  double new_colour  = ((double)colour - 127)/127;

  //shifts the value by contrast
  new_colour *= contrast;
  new_colour = (127*new_colour) + 127;
  colour = (unsigned char)new_colour;

  //checks over/under saturation
  if(new_colour > 255) return ((unsigned char)255);
  if(new_colour < 0) return ((unsigned char)0);
  return colour;
}

/**
   A function that checks over/under saturation and brightens a pixel 

   @param Pixel test a pixel to be tested for saturation
   @param int brightness the amount by which brighness is to be changed
   @return Pixel test the brightened pixel
*/

Pixel check_saturation(Pixel test, int brightness) {

  //changes values for red, green, blue values for colour
  if(test.r + brightness < 0) {
    test.r = 0;
  } else if (test.r + brightness > 255) {
    test.r = 255;
  } else {
    test.r += brightness;
  }

  if(test.g + brightness < 0) {
    test.g = 0;
  } else if (test.g + brightness > 255) {
    test.g = 255;
  } else {
    test.g += brightness;
  }

  if(test.b + brightness < 0) {
    test.b = 0;
  } else if (test.b + brightness > 255) {
    test.b = 255;
  } else {
    test.b += brightness;
  }

  return test; //returns the brightened pixel
    
}

/**
   A function that initializes a gaussian array
   
   @param double sigma the radius of the array
   @return double *gaussarray the double array
*/

double* garray(double sigma) {
  int size = (int)(10 * sigma);
  if (size % 2 == 0) {
    size++;
  }

  //malloc space for the gaussian array
  double *gaussarray = malloc(sizeof(double) * size * size);
  int centre = (size-1) / 2; //index for the centre of the array

  //initialize the array
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      int dx = abs(centre - i); //dx is the horizontal distance from the centre
      int dy = abs(centre - j); //dy is the vertical distance from the centre
      gaussarray[(i * size) + j] = (1.0 / (2.0 * PI * (sigma * sigma))) * exp( -((dx * dx) + (dy * dy)) / (2 * sigma * sigma));
    }
    
  }
  return gaussarray;

}

/**
   A method that determines how far you can go left, right, up, and down from the centre of the gauss array
   
   @param int row the row of the center pixel
   @param int col the column of the center pixel
   @param int size the total size of each dimension of the array
   @param Image *im the image to be checked
   @return int *bounds the int array of max values that can be traveled in each direction
*/
int* checkbounds(int row, int col, int size, Image* im) {
  int dir_size = (size - 1) / 2; //dir_size is the radius

  //calculates the distance you can go in each respective direction
  //basically walks in the direction until you go out of bounds
  int up = 0, down = 0, left = 0, right = 0; 
  for (left = 0; (col - left > 0) && (left < dir_size); left++);
  for (right = 0; (col + right <= im->cols) && (right < dir_size); right++);
  for (up = 0; (row - up > 0) && (up < dir_size); up++);
  for (down = 0; (down + row <= im->rows) && (down < dir_size); down++);

  //return the bounds
  int *bounds = malloc(sizeof(int) * 4);
  bounds[0] = left;
  bounds[1] = right;
  bounds[2] = up;
  bounds[3] = down;
  return bounds;
}

/**
   A method that convolves a gaussian array for a pixel centered at that array
   @param int row the row of the pixel at the center
   @param int col the column of the pixel at the center
   @param size the size of each dimension of the gaussian array
   @param double *gaussarray the gaussian array
   @param Image *im the image to be modified
   @return Pixel blurred the pixel that has the blur applied
*/
Pixel convolve(int row, int col, int* bounds,  int size, double* gaussarray, Image* im) {

  //values for the red, green, blue channels, and the total sum of the gauss array
  double red = 0.0;
  double green = 0.0;
  double blue= 0.0;
  double gauss = 0.0;
  int gauss_index = 0, centre = 0, dx = 0, dy = 0, flag = 0; //index of gauss array, the centre, distance from centre, and a flag to check bounds

  //iterate through the matrix
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      //initialize these variables
      gauss_index = (i * size) + j;
      centre = (size-1)/2;
      dx = i - centre;
      dy = j - centre;
      flag = 0;
      //check left and right
      if(abs(dx) > bounds[2] || abs(dx) > bounds[3]) {
	flag = 1;
      }
      //check down
      if(abs(dy) > bounds[0] || abs(dy) > bounds[1]) {
	flag = 1;
      }
      //check if gauss index is in bounds
      if(gauss_index < 0 || gauss_index > size*size) {
	flag = 1;
      }
      //then add value to weighted average
      if(flag == 0) {
	// change pixel_index based on respective pixel
	int pixel_index = (row + dx)*(im->cols) + (col + dy);
	if (pixel_index >= 0 && pixel_index < (im->cols * im->rows)) {	 
	  //if everything is in bounds, add the values to each respective variable
	  red += ((double)im->data[pixel_index].r) * gaussarray[gauss_index]; 
	  green += ((double) im->data[pixel_index].g) * gaussarray[gauss_index];
	  blue += ((double) im->data[pixel_index].b) * gaussarray[gauss_index];	
	  gauss += gaussarray[gauss_index];
	}
	
      }
    }
  }
  
  //normalize the variables to the gauss matrix
  red = red / gauss;
  green = green / gauss;
  blue = blue / gauss;

  Pixel blurred = im->data[0]; //make a pixel to hold these values

  //check saturation of values
  if (red < 0) {
    red = 0;
  }
  if (red > 255) {
    red = 255;
  }
  if (green < 0) {
    green = 0;
  }
  if (green > 255) {
    green = 255;
  }
  if (blue < 0) {
    blue = 0;
  }
  if (blue > 255) {
    blue = 255;
  }

  //return this pixel
  blurred.r = (unsigned char) red;
  blurred.g = (unsigned char) green;
  blurred.b = (unsigned char) blue;
  return blurred;
}
