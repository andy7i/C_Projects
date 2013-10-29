#ifndef COMPRESS40_C
#define COMPRESS40_C

#include "compress40.h"
#include "arith40.h"




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

struct Coded_Values{ //Need signed ints?
    int a;
    int b;
    int c;
    int d;
    unsigned int pr;
    unsigned int pb;
};
typedef struct Coded_Values coded_values;

extern void compress40  (FILE *input){
    A2Methods_T methods = uarray2_methods_plain;
    Pnm_ppm ppm_file1 = Pnm_ppmread(input, methods);
    A2Methods_UArray2 pixel_array = ppm_file1->pixels;
    A2Methods_UArray2 color_space_array = RGB_to_Color_Space(pixel_array, ppm_file1->denominator);
    A2Methods_UArray2 uncoded_value_array = 
		color_to_uncoded_values(color_space_array);
//    A2Methods_UArray2 coded_value_array = code_values(uncoded_value_array);
    
  //  uncoded_value_array = uncode_values(coded_value_array);
    color_space_array = uncoded_values_to_color(uncoded_value_array);
    pixel_array = Color_Space_to_RGB(color_space_array, ppm_file1->denominator);
    ppm_file1->pixels = pixel_array;
	ppm_file1->width = methods->width(pixel_array);
	ppm_file1->height = methods->height(pixel_array);	///// does this change
fprintf(stderr, "width %d", ppm_file1->width);			///// correctly?
fprintf(stderr, "height %d\n", ppm_file1->height);
    Pnm_ppmwrite(stdout, ppm_file1);
//-    Pnm_ppmfree(&ppm_file1);	// possible leak if width and height are not fixed?
    //methods->free(&color_space_array);
    //methods->free(&uncoded_value_array);

/*
    Pnm_rgb temp_pixel; // = malloc(sizeof(Pnm_rgb));

printf("P3\n%d %d\n %d\n", methods->width(pixel_array), methods->height(pixel_array), ppm_file1->denominator);

    for(int j = 0; j < (int) methods->height(pixel_array); j++){
        for(int i = 0; i < (int) methods->width(pixel_array); i++){
            temp_pixel = methods->at(pixel_array, i, j);
            printf(" %d ", temp_pixel->red);
            printf("%d ", temp_pixel->green);
            printf("%d", temp_pixel->blue);
}
}
*/


    return ;
}


extern void decompress40  (FILE *input){
    (void) input;
    return;
}

//Read-in input 

extern A2Methods_UArray2 read_in_image(FILE *input){
    A2Methods_T methods = uarray2_methods_plain;
    Pnm_ppm ppm_file1 = Pnm_ppmread(input, methods);
    A2Methods_UArray2 image1 = ppm_file1->pixels;
    return image1;
}

//Component - RGB to (Y/Pb/Pr) and inverse
extern A2Methods_UArray2 RGB_to_Color_Space(A2Methods_UArray2 pixel_array, double denom){
//denom = 255;
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(pixel_array) - (methods->width(pixel_array) % 2);
    int height = methods->height(pixel_array) - (methods->height(pixel_array) % 2);

    A2Methods_UArray2 CS_value_array = methods->new(width, height, 
        sizeof(struct Color_Space_values));
    CS_values * temp;// = malloc(sizeof(*temp));
    Pnm_rgb temp_pixel; // = malloc(sizeof(Pnm_rgb));

    double r, g, b; 
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp = methods->at(CS_value_array, i, j);
            temp_pixel = methods->at(pixel_array, i, j);
            r = (double) (temp_pixel->red /denom);
            g = (double) (temp_pixel->green /denom);
            b = (double) (temp_pixel->blue /denom);
            temp->y = (0.299 * r + 0.587 * g + 0.114 * b);
            temp->pb = (-0.168736 * r - 0.331264 * g + 0.5 * b);
            temp->pr = (0.5 * r - 0.418688 * g - 0.081312 * b);

        }
    }
    return CS_value_array;
}

extern A2Methods_UArray2 Color_Space_to_RGB(A2Methods_UArray2 CS_value_array, double denom){
//denom = 255;
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(CS_value_array);
    int height = methods->height(CS_value_array);
    A2Methods_UArray2 pixel_array = methods->new(width, height, 
        sizeof(struct Pnm_rgb));
        
    Pnm_rgb temp_pixel;
    CS_values * temp;
    
    double y, pb, pr; 
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp_pixel = methods->at(pixel_array, i, j);
            temp = methods->at(CS_value_array, i, j);
            y = temp->y;
            pb = temp->pb;
            pr = temp->pr;



            temp_pixel->red = (unsigned) ( ((1.0 * y + 0.0 * pb + 1.402 * pr) * denom));


            (temp_pixel)->green =  (unsigned) ((1.0 * y - 0.344136 * pb - 0.714136 * 
                                pr) * denom);
            (temp_pixel)->blue =  (unsigned) ((1.0 * y + 1.772 * pb + 0.0 * pr) * denom);

if(temp_pixel->red > 255)
	temp_pixel->red = 255;
if(temp_pixel->green > 255)
	temp_pixel->green = 255;
if(temp_pixel->blue > 255)
	temp_pixel->blue = 255;
//if(temp_pixel->red > 255)
//fprintf(stderr, "red pixel = %d denom  = %f pb = %f pr = %f y = %f \n", temp_pixel->red, denom, temp->pb, temp->pr, temp->y);
        }
    }
    return pixel_array;
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
    uncoded_values * temp; 
    for(int j = 0; j < height; j+=2){
        for(int i = 0; i < width; i+=2){
            
            temp = methods->at(uncoded_values_array, i/2, j/2);
            y1 = methods->at(color_space_array, i, j); //Y1
            y2 = methods->at(color_space_array, i+1, j); //Y2
            y3 = methods->at(color_space_array, i, j+1); //Y3
            y4 = methods->at(color_space_array, i+1, j+1); //Y4
            
fprintf(stderr, "y1 %f // ", y1->y);
            temp->a = (y4->y + y3->y + y2->y + y1->y)/4.0;
            temp->b = (y4->y + y3->y - y2->y - y1->y)/4.0;
            temp->c = (y4->y - y3->y + y2->y - y1->y)/4.0;
            temp->d = (y4->y - y3->y - y2->y + y1->y)/4.0;
            temp->pr = (y4->pr + y3->pr + y2->pr + y1->pr)/4.0;
            temp->pb = (y4->pb + y3->pb + y2->pb + y1->pb)/4.0;
            if(temp->a < 0)
               temp->a = 0;
            if(temp->a > 1)
               temp->a = 1;
            if(temp->b > 0.3)
                temp->b = 0.3;
            if(temp->b < -0.3)
                temp->b = -0.3;
            if(temp->c > 0.3)
                temp->c = 0.3;
            if(temp->c < -0.3)
                temp->c = -0.3;
            if(temp->d > 0.3)
                temp->d = 0.3;
            if(temp->d < -0.3)
                temp->d = -0.3; 
fprintf(stderr, "y %f // ", y1->y);
fprintf(stderr, "b %f // ", temp->b);
        }
    }
    return uncoded_values_array;
}

extern A2Methods_UArray2 uncoded_values_to_color(A2Methods_UArray2 
                uncoded_values_array){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(uncoded_values_array)*2;
    int height = methods->height(uncoded_values_array)*2;
    A2Methods_UArray2 color_space_array = methods->new(width, height, 
        sizeof(struct Color_Space_values));
CS_values *y1, *y2, *y3, *y4;
//    CS_values * y1 = malloc(sizeof(*y1));
//    CS_values * y2 = malloc(sizeof(*y2));
//    CS_values * y3 = malloc(sizeof(*y3));
//    CS_values * y4 = malloc(sizeof(*y4));
    uncoded_values * temp; 
	double a, b, c, d, avg_pb, avg_pr;
    for(int j = 0; j < height-1; j+=2){
        for(int i = 0; i < width-1; i+=2){
            temp = methods->at(uncoded_values_array, i/2, j/2);
            a = temp->a;
            b = temp->b;
            c = temp->c;
            d = temp->d;
            avg_pr = temp->pr;
            avg_pb = temp->pb;



            y1 = methods->at(color_space_array, i, j); //Y1
            y2 = methods->at(color_space_array, i+1, j); //Y2
            y3 = methods->at(color_space_array, i, j+1); //Y3
            y4 = methods->at(color_space_array, i+1, j+1); //Y4

			y1->y = a - b - c + d;
			y1->pb = avg_pb;
			y1->pr = avg_pr;
			y2->y = a - b + c - d;
			y2->pb = avg_pb;
			y2->pr = avg_pr;
			y3->y = a + b - c - d;
			y3->pb = avg_pb;
			y3->pr = avg_pr;
			y4->y = a + b + c + d;
			y4->pb = avg_pb;
			y4->pr = avg_pr;
            int check = (1.0 * y1->y + 0.0 * y1->pb + 1.402 * y1->pr)*10000;

             if(check < 0){
fprintf(stderr, "WTH check = %d y = %f a = %f b = %f c = %f d = %f pb = %f pr = %f \n", check, y1->y*10000,  a, b, c, d, avg_pb, avg_pr);
}
        }
    }
    
    return color_space_array;
}

//Component - code the a, b, c, d, Pb, and Pr values.

extern A2Methods_UArray2 code_values(A2Methods_UArray2 uncoded_values_array){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(uncoded_values_array);
    int height = methods->height(uncoded_values_array);
    A2Methods_UArray2 coded_values_array = methods->new(width, height, 
        sizeof(struct Coded_Values));
    coded_values * temp;
    uncoded_values * temp2;
    double a, b, c, d;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp = methods->at(coded_values_array, i, j);
            temp2 = methods->at(uncoded_values_array, i, j);
            a = temp2->a;
            b = temp2->b;
            c = temp2->c;
            d = temp2->d;
//fprintf(stderr, "a1 %f // ", a);
            temp->a = round((a*511));
//fprintf(stderr, "b1 %f // ", b);
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
            temp->b = round((b*50.0));
            temp->c = round((c*50.0));
            temp->d = round((d*50.0));
            temp->pb = Arith40_index_of_chroma(temp2->pb);
            temp->pr = Arith40_index_of_chroma(temp2->pr);
        }
    }        
    return coded_values_array; //change to coded
}

extern A2Methods_UArray2 uncode_values(A2Methods_UArray2 coded_values_array){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(coded_values_array);
    int height = methods->height(coded_values_array);
    A2Methods_UArray2 uncoded_values_array = methods->new(width, height, 
        sizeof(struct Uncoded_Values));
    uncoded_values * temp;
    coded_values * temp2;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp = methods->at(uncoded_values_array, i, j);
            temp2 = methods->at(coded_values_array, i, j);
            temp->a = (temp2->a/511.0);
            temp->b = temp2->b/50.0;
            temp->c = temp2->c/50.0;
            temp->d = temp2->d/50.0;
            temp->pb = Arith40_chroma_of_index(temp2->pb);
            temp->pr = Arith40_chroma_of_index(temp2->pr);

        }
    }
    return uncoded_values_array; 
}

//Component - Pack into words.

extern A2Methods_UArray2 pack_words(A2Methods_UArray2 coded_values_array){
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(coded_values_array);
    int height = methods->height(coded_values_array);
    A2Methods_UArray2 packed_words = methods->new(width, height, 
    sizeof(uint64_t));
    coded_values * temp;
    uint64_t * temp2;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp = methods->at(coded_values_array, i, j);
            temp2 = methods->at(packed_words, i, j);
            (void) temp;
            (void) temp2;
        //DO stuff
        }
    }
   
    return packed_words;
}

extern A2Methods_UArray2 unpack_words(A2Methods_UArray2 packed_words){
    
    A2Methods_T methods = uarray2_methods_plain;
    int width = methods->width(packed_words);
    int height = methods->height(packed_words);
    A2Methods_UArray2 coded_values_array = methods->new(width, height, 
    sizeof(struct Coded_Values));
    coded_values * temp2;
    uint64_t * temp;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            temp = methods->at(packed_words, i, j);
            temp2 = methods->at(coded_values_array, i, j);
            (void) temp;
            (void) temp2;
        //DO stuff
        }
    }
   
    return coded_values_array;
}

//Component - Store words into output file.

extern void store_words(A2Methods_UArray2 packed_words){
    (void) packed_words;
    return;
}

extern A2Methods_UArray2 unstore_words(FILE * input){
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 temp = methods->new(1,1,sizeof(int));
    (void) input;
    return temp;
}

#endif

