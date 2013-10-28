//Eli Kohlenberg and Andrew Jang
//Comp40, Assignment 1, Part A

#ifndef BRIGHTNESS
#define BRIGHTNESS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pnmrdr.h>
#include <except.h>

double avg_numerators (Pnmrdr_T map);
int sum_numerators(Pnmrdr_T map, int size, int denominator);
int tryNew(FILE *input); 

int main(int argc, char *argv[]){
    FILE *input = NULL;
    if (argc > 2){
        fprintf(stderr, "Too many arguments.\n");
        return -1;
    }

    else if (argc == 2) {
        input = fopen(argv[1], "r");
    }

    else if (argc == 1){

    input = stdin;
    }

    if (input != NULL){
        int ret = 0;
        ret = tryNew(input);
        return ret;
    }
    
    else fprintf(stderr, "File not opened succesfully\n");
}

int tryNew(FILE *input) {
    int ret = 0;                        // to indicate success or failure
    Pnmrdr_T image_map = NULL;

    TRY
        image_map = Pnmrdr_new(input);
        double num_avg;
        num_avg = avg_numerators(image_map);
        if (num_avg == -1 || num_avg < 0){
            fclose(input);
            return -1; // this is a check if avg numerators liked file
        }
        printf ("%.3f \n", num_avg);
    EXCEPT(Pnmrdr_Badformat)
        fprintf(stderr, "File not in pnm format.\n");
        ret = -1 ;
    EXCEPT(Pnmrdr_Count)
        fprintf(stderr, "File size is not correct.\n");
        ret = -1 ;
    ELSE
        fprintf(stderr, "Unknown exception raised.\n");
        ret = -1 ;
    END_TRY;
        fclose(input);
        free(image_map);
    return ret;
}

double avg_numerators (Pnmrdr_T map){
    Pnmrdr_mapdata mapdata = Pnmrdr_data(map);
    if (mapdata.type != Pnmrdr_gray){
        fprintf(stderr, "File not graymap.\n");
        return -1;
    }
    int image_size = mapdata.width*mapdata.height;
    int numeratorSum = sum_numerators(map, image_size, mapdata.denominator);
    //passing the denominator is a little awkward, but it seemed like the most
    //efficient solution; we didn't want to re-call Pnmrdr_data, or pass the
    //entire header

    double num_avg = numeratorSum / 
                                ((double)mapdata.denominator * image_size);
    return num_avg;
}

int sum_numerators(Pnmrdr_T map, int size, int denominator) {
    int sum = 0;
    for (int i = 0; i < size; i++){
        int pixel = Pnmrdr_get(map);
        //the check below seemed unnecessary, but it was so easy
        //it results in a large return so that if the file is large,
        //the avg won't get truncated to zero, which would result in
        //a value being printed, possibly a stupid one.
        if (pixel < 0 || pixel > denominator){
            fprintf(stderr, "Pixel not in bounds. File incorrect.\n");
            return -10000;
        }
    sum = sum + pixel;
    }
    return sum;
}


#endif
