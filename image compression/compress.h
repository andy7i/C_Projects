#include <stdio.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pnm.h"
#include "math.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "arith40.h"
#include "bitpack.h"


//Component- Read-in

extern A2Methods_UArray2 read_in_image(FILE *input);

//Component - RGB to (Y/Pb/Pr) and inverse
extern A2Methods_UArray2 RGB_to_Color_Space(A2Methods_UArray2 pixel_array,
                                                               double denom);

//Component - Get a, b, c, d, and avg Pb and Pr values

extern A2Methods_UArray2 color_to_uncoded_values();


//Component - code the a, b, c, d, Pb, and Pr values.

extern A2Methods_UArray2 code_values(A2Methods_UArray2 uncoded_value_array);


//Component - Pack into words.

extern A2Methods_UArray2 pack_words(A2Methods_UArray2 coded_value_array);



//Component - Store words into output file.

extern void store_words(A2Methods_UArray2 packed_words);
