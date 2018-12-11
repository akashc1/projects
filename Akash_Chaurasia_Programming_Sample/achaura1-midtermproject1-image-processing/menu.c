/*
Akash Chaurasia & Parth Vora
achaura1; pvora4
achaura1@jhu.edu; pvora4@jhu.edu
CS.220 Midterm project 1
16/10/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "imagefunctions.h"
#include "pixels.h"
#include "imageIO.h"

//a function that prints out the menu
void print_menu() {
  printf("Main menu:\n");
  printf("\tr <filename> - read image from <filename>\n");
  printf("\tw <filename> - write image to <filename>\n");
  printf("\ts - swap colour channels\n");
  printf("\tbr <amt> - change brightness (up or down) by the given amount\n");
  printf("\tc <x1> <y1> <x2> <y2> - crop image to the box with the given corners\n");
  printf("\tg - convert to grayscale\n");
  printf("\tcn <amt> - change contrast (up or down) by the given amount\n");
  printf("\tbl <sigma> - Gaussian blur with the given radius (sigma)\n");
  printf("\tsh <sigma> <amt> - sharpen by given amount (intensity), with radius (sigma)\n");
  printf("\te <sigma> <threshold> - detect edges with intensity gradient above given threshold\n");
  printf("\tq - quit\n");
  printf("Enter choice: ");
}

//if user input does not match program specs
void bad_input() {
  printf("ERROR: Input does not match required format\n");
}

void error_null() {
  printf("ERROR: No loaded image\n");
}
