#include <stdio.h>
#include "uarray2.h"
#include <stdlib.h>
#include "uarray.h"
#include "assert.h"
#include "uarrayrep.h"
#include "mem.h"
#define T UArray2_T

struct T {
    UArray_T uarray;
    int width;
    int height;
    int size;
};

extern T UArray2_new(int width, int height, int size){
    UArray2_T uarray2;
    NEW(uarray2);
    UArray_T uarray = UArray_new(width*height, size);
    assert((width > 0) && (height > 0) && (size > 0));
    UArray2_init(uarray2, width, height, size, uarray);
    return uarray2;
}

extern void UArray2_init(T uarray2, int width, int height, int size, 
                         UArray_T array){
    uarray2->width = width;
    uarray2->height = height;
    uarray2->size = size;
    uarray2->uarray = array;
}

extern void UArray2_free(T *uarray2){
    assert(uarray2 && *uarray2);
    assert(&((*uarray2)->uarray));
    UArray_free( &((*uarray2)->uarray));
    free(*uarray2);
}

extern int UArray2_width( T uarray2){
    assert(uarray2);
    return uarray2->width;
}

extern int UArray2_height( T uarray2){
    assert(uarray2);
    return uarray2->height;
}

extern int UArray2_size(T uarray2){
    assert(uarray2);
    return uarray2->size;
}

extern void *UArray2_at(T uarray2, int column, int row){
    assert(uarray2);
    assert(column >= 0 && column < uarray2->width);
    assert(row >= 0 && row < uarray2->height);
    return (UArray_at((uarray2->uarray), (row * (uarray2->width) + column)));
}

extern void UArray2_map_row_major(T uarray2, void apply(int width, int height, 
                                  T uarray2, void * elem, void * cl), void *
cl){	
    for(int row = 0; row < uarray2->height; row++){
        for(int colm = 0; colm < uarray2->width; colm++)
            apply(colm, row, uarray2, UArray2_at(uarray2, colm, row), cl);
    }
}

extern void UArray2_map_col_major(T uarray2, void apply(int width, int height, 
                                  T uarray2, void * elem, void * cl), void *
cl){	
    for(int colm = 0; colm < uarray2->width; colm++){
        for(int row = 0; row < uarray2->height; row++)
            apply(colm, row, uarray2, UArray2_at(uarray2, colm, row), cl);
    }
}
#undef T
