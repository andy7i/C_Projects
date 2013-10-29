#ifndef BIT2_C
#define BIT2_C
#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include "bit.h"
#include "bit2.h"

#define T Bit2_T

// 2d bit vector
// invariant: the (row, col) element is represented by (row*width + col)

struct T
{
    int width;
    int height;
    int area;
    Bit_T vector;
};

// Creates an unboxed array of dimensions width * height (= size)
// Note that height is num rows, width is num cols
// returns a pointer to the unboxed array
T Bit2_new(int height, int width)
{
    //T bit2 = malloc(sizeof(T));
    T bit2;
    NEW(bit2);
    Bit_T vector = Bit_new(height * width);
    bit2 -> vector = vector;
    bit2 -> height = height;
    bit2 -> width = width;
    bit2 -> area = width * height;
    return bit2;
}

void Bit2_free(T *bit2)
{
    Bit_free(&(*bit2) -> vector);
    FREE (*bit2);
}

int Bit2_height(T bit2)
{
    return bit2 -> height;
}

int Bit2_width(T bit2)
{
    return bit2 -> width;
}

int Bit2_area(T bit2)
{
    return bit2 -> area;
}

int Bit2_get(T bit2, int row, int col)
{
    return Bit_get(bit2 -> vector, (row * (bit2 -> width) + col));
}

int Bit2_put(T bit2, int row, int col, int bit)
{
    return Bit_put(bit2 -> vector, (row * (bit2 -> width) + col), bit);
}

void Bit2_map_row_major(T bit2, int apply(int elem, void* cl, 
                int row, int col, T bit2), void* cl)
{
    int i;
    for(i = 0; i < bit2 -> area; i++)
    {
        Bit_put(bit2 -> vector, i, 
            apply(Bit_get(bit2 -> vector, i), cl, (i / bit2->width), 
                        (i % bit2->width), bit2));
    }
}

void Bit2_map_col_major(T bit2, int apply(int elem, void* cl, 
                        int row, int col, T bit2), void* cl)
{
    int i, j;
    for(j = 0; j < bit2 -> width; j++)
    {
        for(i = 0; i < bit2 -> height; i++)
        {
            int iter;
            iter = i * (bit2 -> width) + j;
            Bit_put(bit2 -> vector,iter,
                apply(Bit_get(bit2 -> vector, iter), cl, i, j, 
                                    bit2));
        }
        
    }
}
    

#endif
