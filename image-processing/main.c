#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "imagefunctions.h"
#include "pixels.h"
#include "imageIO.h"

int main(void) {
  char function[2] = "aa"; //holds which function the user wants 
  char input[1024] = "a"; //holds user input
  Image *orig; // declare image pointer to be used for everything

  do {
    //print menu and get user input
    print_menu();
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%s", function);
    int check = 0;

    /* READING FUNCTION */
    if(strcmp(function, "r") == 0) {
      char readfile[1024];
      char throwaway[1024];
      check = sscanf(input, "%s %s", throwaway,  readfile);
      printf("Reading from %s...\n", readfile);
      if(check != 2) {
	bad_input();
      } else {
	FILE *fp = fopen(readfile, "r");
	if (fp == NULL) {
	  printf("Error:ppmIO - failed to open \"%s\" for reading\n", readfile);
	} else {
	  // call read file from imageIO files
	  orig = readPPMImage(fp);
	  // close file!
	  fclose(fp);
	}
      }

    /* WRITING FUNCTION */
    } else if(strcmp(function, "w") == 0) {
      char writefile[1024] = "a";
      char throwaway[1024] = "a";
      check = sscanf(input, "%s %s", throwaway, writefile);
      if(check != 2) {
        bad_input();
      } else {
	/* CHECK THE FUNCTIONS */
	if (orig == NULL) {
	  error_null();
	} else {
	  
	  FILE *fp = fopen(writefile, "w");
	  check = writePPMImage(orig, fp);
	  printf("Writing to %s...\n", writefile);
	  // if for some reason file writing is not successful
	  if(check != (orig->rows * orig->cols)) {
	    printf("File writing not successful\n");
	  }
	  
	// close file!
	  fclose(fp);
	}
      }

      /*SWAP COLOUR CHANNELS */
    } else if(strcmp(function, "s") == 0) {
      // call color swap function
      printf("Swapping color channels...\n");
      colour_swap(orig);

      /* CHANGE BRIGHTNESS */
    } else if(strcmp(function, "br") == 0) {
      int brightness = 0;
      char throwaway[1024];
      check = sscanf(input, "%s %d", throwaway, &brightness);
      if(check != 2) {
        bad_input();
      } else {
	// call brightness function
	printf("Adjusting brightness by %d...\n", brightness);
	change_brightness(orig, brightness);
      }

      /* CROP IMAGE */
    } else if(strcmp(function, "c") == 0) {
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
      char throwaway[1024] = "a";
      check = sscanf(input, "%s %d %d %d %d", throwaway, &x1, &y1, &x2, &y2);
      if(check != 5) {
        bad_input();
      } else {
	printf("Cropping region from (%d, %d) to (%d, %d)...\n", x1, y1, x2, y2);
        Image *temp = crop(orig, x1, y1, x2, y2);

	// switch to new image if the function was successful
	if(temp != NULL) {
	  free(orig->data);
	  free(orig);
	  orig = temp;
	}
      }

      /* TO GREYSCALE */
    } else if(strcmp(function, "g") == 0) {
      // call greyscale function
      printf("Converting to grayscale...\n");
      greyscale(orig);
      
      /* CHANGE CONTRAST */
    } else if(strcmp(function, "cn") == 0) {
      double contrast = 0.0;
      char throwaway[1024] = "a";
      check = sscanf(input, "%s %lf", throwaway, &contrast);
      if(check != 2) {
        bad_input();
      } else {
	printf("Adjusting contrast by %.2lf...\n", contrast);
	// call contrast function
        change_contrast(orig, contrast);
      }

      /*GAUSSIAN BLUR */
    } else if(strcmp(function, "bl") == 0) {
      double sigma = 0.0;
      char throwaway[1024] = "a";
      
      check = sscanf(input, "%s %lf", throwaway, &sigma);
      if(check != 2) {
        bad_input();
      } else {
	printf("Applying blur filter, sigma %.2lf...\n", sigma);
	// swap image pixel arrays
	Pixel *temp = blur(orig, sigma);
	if(temp != NULL) {
	  free(orig->data);
	  orig->data = temp;
	}
      }

      /*SHARPEN */
    } else if(strcmp(function, "sh") == 0) {
      char throwaway[1024] = "a";
      double sigma = 0.0;
      double strength = 0.0;
      check = sscanf(input, "%s %lf %lf", throwaway, &sigma, &strength);
      if(check != 3) {
        bad_input();
      } else {
	printf("Applying sharpen filter, sigma %.2lf, intensity %.2lf...\n", sigma, strength);
	// swap image pixel arrays
	Pixel *sharp = sharpen(sigma, strength, orig);
	if(sharp != NULL) {
	  free(orig->data);
	  orig->data = sharp;
	}

      }

      /*DETECT EDGES */
    } else if(strcmp(function, "e") == 0) {
      double threshold = 0.0;
      double sigma = 0.0;
      char throwaway[1024] = "a";
      check = sscanf(input, "%s %lf %lf", throwaway, &sigma, &threshold);
      if(check != 3) {
        bad_input();
      } else {
	printf("Doing edge detection with sigma %.2lf and threshold %.2lf...\n", sigma, threshold);
	// call edge detect function
	detect_edges(orig, sigma, threshold);
      }

    } else if(strcmp(function, "q") == 0) {
      printf("Goodbye!\n");
    } else {
      printf("Invalid input!\n");
    }
    
  } while(strcmp(function, "q") != 0);
  // free image if it has any contents
  if (orig != NULL) {
    free(orig->data);
    free(orig);
  }
  return 0;
}
