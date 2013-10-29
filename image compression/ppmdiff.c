#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pnm.h"
#include "math.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"


int main(int argc, char* argv[]) {

FILE * fp = NULL;
if (argc == 1 || argc == 2 || argc > 3){ 
  fprintf(stderr, "Please enter the correct number of arguments(3).\n"); 
  exit(EXIT_FAILURE);
}    


  A2Methods_T methods = uarray2_methods_plain; // default to UArray2 methods
  assert(methods);
  A2Methods_mapfun *map = methods->map_default; // default to best map
  assert(map);

Pnm_ppm ppm_file1;
Pnm_ppm ppm_file2;

  if (!strcmp(argv[1], "-") && !strcmp(argv[2], "-")){
  fprintf(stderr, "Error please specify an image not from stdin.\n");
  exit(EXIT_FAILURE);
  }


    
    if (!strcmp(argv[1], "-")) {
      ppm_file1 = Pnm_ppmread(stdin, methods);
      fp = fopen(argv[2], "r");
      if(fp == NULL) {exit(1);}
      ppm_file2 = Pnm_ppmread(fp, methods);
      fclose(fp);
     }
      else{
           fp = fopen(argv[1], "r");
           if(fp == NULL){ exit(1);}
           ppm_file1 = Pnm_ppmread(fp, methods);
           fclose(fp);

          if (!strcmp(argv[2], "-")) 
            ppm_file2 = Pnm_ppmread(stdin, methods);
          else{
            fp = fopen(argv[2], "r");
            if(fp == NULL){ exit(1);}
            ppm_file2 = Pnm_ppmread(fp, methods);
            fclose(fp);
          }
       }
A2Methods_UArray2 image1 = ppm_file1->pixels;
A2Methods_UArray2 image2 = ppm_file2->pixels;
int height1 = methods->height(image1);
int height2 = methods->height(image2);
int width1 = methods->width(image1);
int width2 = methods->width(image2);
if(abs(height1 - height2) > 1 ||  abs(width1 - width2) > 1){
	fprintf(stderr, "dimensions: h%d, %d; w%d, %d\n", height1, height2, width1, width2);
    fprintf(stderr, "Dimesions of images are differnt.\n");
    fprintf(stdout, "%lf", 1.0);
    exit(EXIT_FAILURE);
}

int height, width;
if(height1 < height2)
   height = height1;
else
   height = height2;
if(width1 < width2)
   width = width1;
else
   width = width2;

Pnm_rgb  temp1;
Pnm_rgb  temp2;
double r1 = 0;
double r2 = 0;
double g1 = 0;
double g2 = 0;
double b1 = 0;
double b2 = 0;
double sum = 0.0;
for(int j = 0; j < height; j++){
    for(int i = 0; i < width; i++){
       temp1 = methods->at(image1, i, j);
	   r1 = temp1->red/255.0;
	   g1 = temp1->green/255.0;
	   b1 = temp1->blue/255.0;
       temp2 = methods->at(image2, i, j);
	   r2 = temp2->red/255.0;
	   g2 = temp2->green/255.0;
	   b2 = temp2->blue/255.0;
	sum += pow(((r1 - r2)), 2.0)/(3.0*width*height);
	sum += pow(((g1 - g2)), 2.0)/(3.0*width*height);
	sum += pow(((b1 - b2)), 2.0)/(3.0*width*height);
   }
}
double answer = sqrt(sum);
printf("%.4f\n", answer );
//Pnm_ppmwrite(stdout, ppm_file1);
//methods->free(&im	age1);
//methods->free(&image2);
Pnm_ppmfree(&ppm_file1);
Pnm_ppmfree(&ppm_file2);

}

