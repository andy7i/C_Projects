#ifndef UARRAY2_C
#define UARRAY2_C

#include <stdlib.h>
#include <stdio.h>
#include "except.h"
#include "mem.h"
#include "assert.h"
#include "uarray.h"
#include "uarray2.h"

#define T UArray2_T

//Creates 2d Uarray
// invariant: elements always stored in the form of row, col.

struct T
{
    int height;
    int width;
    int size;  // num bytes in memory per elem
    UArray_T array;
};

// Creates an unboxed array of dimensions width * height (= size)
// Note that height is num rows, width is num cols
// returns a pointer to the unboxed array
T UArray2_new(int height, int width, int size)
{
    T uarray2 = malloc(sizeof(*uarray2));
    //RAISE(Mem_Failed);
    uarray2 -> array = UArray_new(height, (sizeof(UArray_T*)));
    int i;
    for(i = 0; i < height; i++)
    {
        //UArray_T uarray = malloc(size);         // necessary?
       // UArray_T uarray = UArray_new(width, size);
        UArray_T* row = UArray_at(uarray2 -> array, i);
        *row = UArray_new(width, size);
    int j;
    for(j = 0; j < width; j++)
    {
        int* elemp = UArray_at(*row, j);
        *elemp = 0;
    }
    
    }
    uarray2 -> height = height;
    uarray2 -> width = width;
    uarray2 -> size = size;
    return uarray2;
}

void UArray2_free(T *uarray2)
{
    int i;
    for(i = 0; i < (*uarray2) -> height; i++)
    {
        UArray_free(UArray_at((*uarray2) -> array, i));
    }
    UArray_free(&(*uarray2) -> array);
    FREE (*uarray2);
    return;
}
//UArray
void *UArray2_at(T array2, int row, int col)
{
    UArray_T* p = UArray_at(array2 -> array, row);
    //assert(sizeof(*p)) == UArray_size(uarray2 -> array);
    UArray_T inner_array = *p;
    int* q = UArray_at(inner_array, col);
    return (int*)q;
}

int UArray2_size(T array2)
{
    return array2 -> size;
}

void UArray2_map_row_major(T array2, void apply(void* elem, void* cl, 
                int row, int col, T array), void* cl)
{
    int i, j;
    for(i = 0; i < array2->height; i++)
    {
        for(j = 0; j < array2->width; j++)
        {
            apply((UArray2_at(array2, i, j)), cl, i, j, array2);
    }
    }
}

void UArray2_map_col_major(T array2, void apply(void* elem, void* cl, 
                int row, int col, T array), void* cl)
{
    int j, i;
    for(j = 0; j < array2->width; j++)
    {
        for(i = 0; i < array2->height; i++)
        {
            apply((UArray2_at(array2, i, j)), cl, i, j, array2);
        }
    }
}

#endif
