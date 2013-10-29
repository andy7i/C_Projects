#ifndef COMPRESS40_H
#define COMPRESS40_H

#include <stdio.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pnm.h"
#include "math.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"


/* The two functions below are functions you should implement.
   They should take their input from the parameter and should
   write their output to stdout */

extern void compress40(FILE *input); // reads PPM and writes a compressed image

extern void decompress40(FILE *input); // reads a compressed image and writes PPM

//Component- Read-in

extern A2Methods_UArray2 read_in_image(FILE *input);

//Component - RGB to (Y/Pb/Pr) and inverse
extern A2Methods_UArray2 RGB_to_Color_Space(A2Methods_UArray2 pixel_array, double denom);

extern A2Methods_UArray2 Color_Space_to_RGB(A2Methods_UArray2 CS_value_array, double denom);

//Component - Get a, b, c, d, and avg Pb and Pr values

extern A2Methods_UArray2 color_to_uncoded_values();

extern A2Methods_UArray2 uncoded_values_to_color();

//Component - code the a, b, c, d, Pb, and Pr values.

extern A2Methods_UArray2 code_values(A2Methods_UArray2 uncoded_value_array);

extern A2Methods_UArray2 uncode_values(A2Methods_UArray2 coded_value_array);

//Component - Pack into words.

extern A2Methods_UArray2 pack_words(A2Methods_UArray2 coded_value_array);

extern A2Methods_UArray2 unpack_words(A2Methods_UArray2 packed_words);

//Component - Store words into output file.

extern void store_words(A2Methods_UArray2 packed_words);

extern A2Methods_UArray2 unstore_words(FILE *input);

#endif
