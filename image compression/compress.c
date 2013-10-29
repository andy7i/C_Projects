#ifndef COMPRESS_C
#define COMPRESS_C

#include "compress.h"

typedef struct Color_Space_values{
    double y;
    double pb;
    double pr;
} CS_values;

struct Uncoded_Values{
    double a;
    double b;
    double c;
    double d;
    double pr;
    double pb;
};
typedef struct Uncoded_Values uncoded_values;

struct Coded_Values{ 
    int a;
    int b;
    int c;
    int d;
    unsigned int pr;
    unsigned int pb;
};
typedef struct Coded_Values coded_values;

//Read-in input 

extern A2Methods_UArray2 read_in_image(FILE *input){
    A2Methods_T methods = uarray2_methods_plain;
    Pnm_ppm ppm_file1 = Pnm_ppmread(input, methods);
    A2Methods_UArray2 image1 = ppm_file1->pixels;
    return image1;
}

//Component - RGB to (Y/Pb/Pr) and inverse
extern A2Methods_UArray2 RGB_to_Color_Space(A2Methods_UArray2 pixel_array, double denom){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(pixel_array) - (methods->width(pixel_array) % 2);
    int height = methods->height(pixel_array) - (methods->height(pixel_array) % 2);

    A2Methods_UArray2 CS_value_array = methods->new(width, height, 
        sizeof(struct Color_Space_values));
    CS_values * temp_CS;
    Pnm_rgb temp_pixel;

    double r, g, b; 
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp_CS = methods->at(CS_value_array, i, j);
            temp_pixel = methods->at(pixel_array, i, j);
            r = (double) (temp_pixel->red /denom);
            g = (double) (temp_pixel->green /denom);
            b = (double) (temp_pixel->blue /denom);

            temp_CS->y = (0.299 * r + 0.587 * g + 0.114 * b);
            temp_CS->pb = (-0.168736 * r - 0.331264 * g + 0.5 * b);
            temp_CS->pr = (0.5 * r - 0.418688 * g - 0.081312 * b);

        }
    }
    return CS_value_array;
}

//Component - Get a, b, c, d, and avg Pb and Pr values

extern A2Methods_UArray2 color_to_uncoded_values(A2Methods_UArray2 
                color_space_array){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(color_space_array);
    int height = methods->height(color_space_array);
    A2Methods_UArray2 uncoded_values_array = methods->new(width/2, height/2, 
        sizeof(struct Uncoded_Values));
    CS_values *  y1, * y2, * y3, * y4;
    uncoded_values * temp_uncoded; 
    for(int j = 0; j < height; j+=2){
        for(int i = 0; i < width; i+=2){
            
            temp_uncoded = methods->at(uncoded_values_array, i/2, j/2);
            y1 = methods->at(color_space_array, i, j); //Y1
            y2 = methods->at(color_space_array, i+1, j); //Y2
            y3 = methods->at(color_space_array, i, j+1); //Y3
            y4 = methods->at(color_space_array, i+1, j+1); //Y4
            temp_uncoded->a = (y4->y + y3->y + y2->y + y1->y)/4.0;
            temp_uncoded->b = (y4->y + y3->y - y2->y - y1->y)/4.0;
            temp_uncoded->c = (y4->y - y3->y + y2->y - y1->y)/4.0;
            temp_uncoded->d = (y4->y - y3->y - y2->y + y1->y)/4.0;
            temp_uncoded->pr = (y4->pr + y3->pr + y2->pr + y1->pr)/4.0;
            temp_uncoded->pb = (y4->pb + y3->pb + y2->pb + y1->pb)/4.0;
            if(temp_uncoded->a < 0)
               temp_uncoded->a = 0;
            if(temp_uncoded->a > 1)
               temp_uncoded->a = 1;
            if(temp_uncoded->b > 0.3)
                temp_uncoded->b = 0.3;
            if(temp_uncoded->b < -0.3)
                temp_uncoded->b = -0.3;
            if(temp_uncoded->c > 0.3)
                temp_uncoded->c = 0.3;
            if(temp_uncoded->c < -0.3)
                temp_uncoded->c = -0.3;
            if(temp_uncoded->d > 0.3)
                temp_uncoded->d = 0.3;
            if(temp_uncoded->d < -0.3)
                temp_uncoded->d = -0.3; 
        }
    }
    return uncoded_values_array;
}

//Component - code the a, b, c, d, Pb, and Pr values.

extern A2Methods_UArray2 code_values(A2Methods_UArray2 uncoded_values_array){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(uncoded_values_array);
    int height = methods->height(uncoded_values_array);
    A2Methods_UArray2 coded_values_array = methods->new(width, height, 
        sizeof(struct Coded_Values));
    coded_values * temp_coded;
    uncoded_values * temp_uncoded;
    double a, b, c, d;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp_coded = methods->at(coded_values_array, i, j);
            temp_uncoded = methods->at(uncoded_values_array, i, j);
            a = temp_uncoded->a;
            b = temp_uncoded->b;
            c = temp_uncoded->c;
            d = temp_uncoded->d;
            if(a > 1.0)
                a = 1.0;
            if(a < 0)
                a = 0;
            if(b > 0.3)
                b = 0.3;
            if(b < -0.3)
                b = -0.3;
            if(c > 0.3)
                c = 0.3;
            if(c < -0.3)
                c = -0.3;
            if(d > 0.3)
                d = 0.3;
            if(d < -0.3)
                d = -0.3;
            temp_coded->a = round((a*511));
            temp_coded->b = round((b*50.0));
            temp_coded->c = round((c*50.0));
            temp_coded->d = round((d*50.0));
            temp_coded->pb = Arith40_index_of_chroma(temp_uncoded->pb);
            temp_coded->pr = Arith40_index_of_chroma(temp_uncoded->pr);
        }
    }        
    return coded_values_array;
}

//Component - Pack into words.

extern A2Methods_UArray2 pack_words(A2Methods_UArray2 coded_values_array){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(coded_values_array);
    int height = methods->height(coded_values_array);
    A2Methods_UArray2 packed_words = methods->new(width, height, 
    sizeof(uint64_t));
    coded_values * temp_coded;
    uint64_t * temp_uint;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp_coded = methods->at(coded_values_array, i, j);
            temp_uint = methods->at(packed_words, i, j);
            *temp_uint = 0;
            *temp_uint = Bitpack_newu(*temp_uint, 9, 23, (uint64_t) temp_coded->a);
            *temp_uint = Bitpack_news(*temp_uint, 5, 18, (int64_t) temp_coded->b);
            *temp_uint = Bitpack_news(*temp_uint, 5, 13, (int64_t) temp_coded->c);
            *temp_uint = Bitpack_news(*temp_uint, 5, 8, (int64_t) temp_coded->d);
            *temp_uint = Bitpack_newu(*temp_uint, 4, 4, (uint64_t) temp_coded->pb);
            *temp_uint = Bitpack_newu(*temp_uint, 4, 0, (uint64_t) temp_coded->pr);
        }
    }
   
    return packed_words;
}

//Component - Store words into output file.

extern void store_words(A2Methods_UArray2 packed_words){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(packed_words);
    int height = methods->height(packed_words);
    fprintf(stdout, "COMP40 Compressed image format 2\n%u %u\n", width, height);
    uint64_t * temp_uint;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp_uint = methods->at(packed_words, i, j);
            for(int x = 3; x > -1; x--)       
            putchar(Bitpack_gets(*temp_uint, 8, 8*x));
        }
    }     
    return;
}

#endif
