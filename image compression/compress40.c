#ifndef COMPRESS40_C
#define COMPRESS40_C

#include "compress40.h"
#include "arith40.h"
#include "bitpack.h"
#include "compress.c"
#include "decompress.c"




extern void compress40  (FILE *input){

   A2Methods_T methods = uarray2_methods_plain;
   Pnm_ppm ppm_file1 = Pnm_ppmread(input, methods);
   A2Methods_UArray2 pixel_array = ppm_file1->pixels;
   int denom = ppm_file1->denominator;
   
   A2Methods_UArray2 color_space_array = RGB_to_Color_Space(pixel_array, denom);
   Pnm_ppmfree(&ppm_file1);
   
   A2Methods_UArray2 uncoded_value_array = color_to_uncoded_values(color_space_array);
   methods->free(&color_space_array);
   
   A2Methods_UArray2 coded_value_array = code_values(uncoded_value_array); 
   methods->free(&uncoded_value_array);  

   A2Methods_UArray2 packed_words = pack_words(coded_value_array);
   methods->free(&coded_value_array);

   store_words(packed_words);
   methods->free(&packed_words);

   return ;
}

extern void decompress40  (FILE *input){
    A2Methods_T methods = uarray2_methods_plain;
    int denom = 255;
    A2Methods_UArray2 packed_words = unstore_words(input);

    A2Methods_UArray2 coded_value_array = unpack_words(packed_words);
    methods->free(&packed_words);
 
    A2Methods_UArray2 uncoded_value_array = uncode_values(coded_value_array);
    methods->free(&coded_value_array);
    
    A2Methods_UArray2 color_space_array = uncoded_values_to_color(uncoded_value_array);
    methods->free(&uncoded_value_array);  

    A2Methods_UArray2 pixel_array = Color_Space_to_RGB(color_space_array, denom);
    methods->free(&color_space_array);

    Pnm_ppm ppm_file1 = malloc(sizeof(struct Pnm_ppm));
    ppm_file1->pixels = pixel_array;
    ppm_file1->methods = methods;
    ppm_file1->denominator = denom;
    ppm_file1->width = methods->width(pixel_array);
    ppm_file1->height = methods->height(pixel_array);
    Pnm_ppmwrite(stdout, ppm_file1);
    Pnm_ppmfree(&ppm_file1);
    return;
}


#endif

