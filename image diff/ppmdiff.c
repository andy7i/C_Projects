#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pnm.h"
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


   FILE * fp = NULL;

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
}

