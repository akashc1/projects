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


int writePPM(Pixel *image, int rows, int cols, int colors, FILE *fp) {
  // if for some reason file is not open
  if (!fp) {
    fprintf(stderr, "Error:ppmIO - writePPM given a bad filehandle\n");
    return 0;
  }
  
  //write tag 
  fprintf(fp, "P6\n");

  // write dimensions 
  fprintf(fp, "%d %d\n%d\n", cols, rows, colors);

  // write pixels 
  int written = fwrite(image, sizeof(Pixel), rows * cols, fp);
  if (written != (rows * cols)) {
    fprintf(stderr, "ERROR: Unable to write file\n");
  }

  return written;
}


// Wrapper function that calls writePPM
int writePPMImage(Image *im, FILE *fp) {
  return writePPM(im->data, im->rows, im->cols, im->colors, fp);
}

//READ PPM IMAGE
Image* readPPMImage(FILE *fp) {
  if(!fp) {
    printf("ERROR: File not found\n");
  }

  //check that file is P6
  char P6[] = "aa";
  P6[0] = getc(fp);
  P6[1] = getc(fp);
  if(strcmp(P6, "P6") != 0) {
    printf("ERROR: File is not P6\n");
    return NULL;
  }

  while(getc(fp) != '\n');

  //remove comments
  char comment = getc(fp);
  while(comment == '#') {
    while(getc(fp) != '\n'); //go to newline character
    comment = getc(fp);
  }

  ungetc(comment, fp);

  int width = 0;
  if(fscanf(fp, "%d ", &width) != 1) {
    printf("ERROR: File does not have specified width\n");
    return NULL;
  }

  comment = getc(fp);
  while(comment == '#') {
    while(getc(fp) != '\n'); //go to newline character
    comment = getc(fp);
  }
  ungetc(comment, fp);


  int height = 0;
  if(fscanf(fp, "%d", &height) != 1) {
    printf("ERROR: File does not have specified width\n");
    return NULL;
  }
  while (getc(fp) != '\n');
  /*
  //read image width and height
  int width = 0, height = 0;
  if(fscanf(fp, "%d %d", &width, &height) != 2) {
    printf("ERROR: File does not have specified dimensions\n");
    return NULL;
  }
  */
  comment = getc(fp);
  while(comment == '#') {
    while(getc(fp) != '\n'); //go to newline character
    comment = getc(fp);
  }
  ungetc(comment, fp);
  

  //Ensure colour shades = 255
  int shades = 0;
  if(fscanf(fp, "%d", &shades) != 1) {
    printf("Error: File has not specified number of shades\n");
    return NULL;
  }
  if(shades != 255) {
    printf("Error: File number of shades is not 255\n");
    return NULL;
  }

  while (getc(fp) != '\n');
  comment = getc(fp);
  while(comment == '#') {
    while(getc(fp) != '\n'); //go to newline character
    comment = getc(fp);
  }
  ungetc(comment, fp);
  

  //move past new line
  //while(fgetc(fp) != '\n');

  //Initialize image
  Image *loaded_image = malloc(sizeof(Image)); 
  loaded_image->data = malloc(sizeof(Pixel) * width * height); 
  loaded_image->rows = height;
  loaded_image->cols = width;
  loaded_image->colors = shades;
  // load file data into dynamically allocated memory
  fread(loaded_image->data, sizeof(Pixel), width * height, fp);
  return loaded_image;
  
} 
