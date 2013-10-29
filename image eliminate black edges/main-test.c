#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include <except.h>
#include "uarray2.h"
#include "assert.h"

void readInPixels (void* elem, void* image_map, int row, int col, UArray2_T
        array);


int main(int argc, char *argv[])
{
    (void) argc;
    FILE *input = NULL;
    Pnmrdr_T image_map = NULL;
        input = fopen(argv[1], "r");
    
        image_map = Pnmrdr_new(input);
        UArray2_T sudoku = UArray2_new(9,9, sizeof(int));

        void (*pt2readInPixels)(void*, void*, int, int, UArray2_T);
        pt2readInPixels = readInPixels;
        UArray2_map_row_major (sudoku, pt2readInPixels, image_map);


}

void readInPixels (void* elem, void* image_map, int row, int col, UArray2_T
        array)
{
    (void) row;
    (void) col;
    (void) array;
printf("counts");
    int pixel = Pnmrdr_get(image_map);
//    assert(pixel >= PIXEL_MIN && pixel <= PIXEL_MAX);
    printf("pixel: %d\n", pixel);
    *(int*)elem = pixel;
    printf("%d\n", *(int*)elem);
    printf("%p\n", elem);
    printf("%p\n", UArray2_at(array, row, col));
}
