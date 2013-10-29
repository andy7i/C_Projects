// Andrew Jang and James Downe
// Comp40, Assignment 2, Part C

#ifndef SUDOKU_C
#define SUDOKU_C
#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include <except.h>
#include "uarray2.h"
#include "assert.h"

const int MAX_WIDTH = 9;
const int MAX_HEIGHT = 9;
const int PIXEL_MAX = 9;
const int PIXEL_MIN = 1;
const int BLOCK_WIDTH = 3;  // must be able to (SUDOKU % BLOCK == 0)
const int BLOCK_HEIGHT = 3;
const int SUM_OF_1_9 = 45;
const int FACTORIAL_9 = 362880;

int readGrid(FILE *input);
int buildArray (Pnmrdr_T image_map);
static void readInPixels (void* elem, void* image_map, int row, int col, 
                                                UArray2_T array);
int checkLines (UArray2_T sudoku);
int checkBlocks (UArray2_T sudoku);


int main(int argc, char *argv[])
{
    FILE *input = NULL;
    int ret = 1;
    if (argc > 2)
    {
        fprintf(stderr, "Too many arguments.\n");
        return -1;
    }

    else if (argc == 2) 
    {
        input = fopen(argv[1], "r");
printf("here");
    }

    else if (argc == 1)
    {
        input = stdin;
    }

    if (input != NULL)
    {
        ret = readGrid(input);
    }
    
    else fprintf(stderr, "File not opened succesfully\n");

    exit (ret);
}


int readGrid (FILE *input)
{
    int ret = 1;
    Pnmrdr_T image_map = NULL;
    
    TRY
        image_map = Pnmrdr_new(input);
        ret = buildArray(image_map);
    EXCEPT(Pnmrdr_Badformat)
        fprintf(stderr, "File not in pnm format.\n");
        ret = 1;
    EXCEPT(Pnmrdr_Count)
        fprintf(stderr, "File size is not correct.\n");
        ret = 1 ;
    ELSE
        fprintf(stderr, "Unknown exception raised.\n");
        ret = 1 ;
    END_TRY;
        fclose(input);
        free(image_map);
    return ret;
}

int buildArray (Pnmrdr_T image_map)
{
    Pnmrdr_mapdata mapdata = Pnmrdr_data(image_map);
    
    if (mapdata.type != Pnmrdr_gray)
    {
        fprintf(stderr, "File not graymap.\n");
        return 1;
    }

    if ((int)mapdata.width == MAX_WIDTH && (int) mapdata.height == MAX_HEIGHT)
    {
        UArray2_T sudoku = UArray2_new(MAX_HEIGHT,MAX_WIDTH, sizeof(int));
        
        void (*pt2readInPixels)(void*, void*, int, int, UArray2_T);
        pt2readInPixels = readInPixels;
        UArray2_map_row_major (sudoku, readInPixels, image_map);
    if(checkLines(sudoku) + checkBlocks(sudoku) == 0)
    {
        UArray2_free(&sudoku);
        return 0;
    }
        UArray2_free(&sudoku);
    }

    return 1;
}

// "apply" function for map_ func
void readInPixels (void* elem, void* image_map, int row, int col, UArray2_T
        array)
{
    (void) row;
    (void) col;
    (void) array;
    int pixel = Pnmrdr_get(image_map);
    if(pixel < PIXEL_MIN || pixel > PIXEL_MAX)
    {
        pixel = 0;
        fprintf(stderr, "Pixel not within bounds, file incorrect\n");
    }
    *(int*)elem = pixel;
}

int checkLines(UArray2_T sudoku)
{
    int colSum, rowSum, colProduct, rowProduct, i, j;
    for (i=0; i < MAX_HEIGHT; i++)
    {
        colSum = 0;
        rowSum = 0;
        colProduct = 1;
        rowProduct = 1;

        for (j=0; j < MAX_WIDTH; j++)
        {
            colSum = colSum + ((*(int*)UArray2_at(sudoku, i, j)));
            rowSum = rowSum + *(int*)UArray2_at(sudoku, j, i);
            colProduct = colProduct * *(int*)UArray2_at(sudoku, i, j);
            rowProduct = rowProduct * *(int*)UArray2_at(sudoku, j, i);
        }
        
        if(!(colSum == SUM_OF_1_9 && rowSum == SUM_OF_1_9
                && colProduct == FACTORIAL_9 && rowProduct == FACTORIAL_9))
        return 1;
    }
    return 0;
}

int checkBlocks(UArray2_T sudoku)
{
    int blockSum, blockProduct, a, b, i, j;

    for (a = 0; a < MAX_HEIGHT/BLOCK_HEIGHT; a++)
    {
        for (b = 0; b < MAX_WIDTH/BLOCK_WIDTH; b++)
        {
        blockSum = 0;
                blockProduct = 1;
            for (i=(a*3); i < (a*3 + 3); i++)
            {
                for (j=(b*3); j < (b*3 + 3); j++)
                {
                  blockSum = blockSum + ((*(int*)UArray2_at(sudoku, i, j)));
                  blockProduct = blockProduct * *(int*)UArray2_at(sudoku, i, j);
                }

            }
             if(!(blockSum == SUM_OF_1_9 && blockProduct == FACTORIAL_9 ))
            return 1;
        }
    }
    return 0;

}

#endif
