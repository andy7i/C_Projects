#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include "uarray.h"
#define T UArray2_T
typedef struct T *T;

extern T UArray2_new(int width, int height, int size);
/* Creates a 2D array with dimension 'width' by 'height'.If 'width' or 'height' 
is 0, the array contains 0 elements. It is a checked run-time error for 'width'
or 'height' ot be negative. Each element in the array has a size 'size'. It is a
checked run-time error for size to be 0 or negative. It returns the new 2D
array. Each element's location can be represented by the a pair of indices which
can be called by other functions. These indices range from [0][0] to
[width-1][height-1].
*/

extern void UArray2_init(T uarray2, int width, int height, int size, 
                         UArray_T array);
/*
This function is called in UArray2_new, and it will initalize all the elements 
in the UArray2 struct.
*/

extern void UArray2_free(T *uarray2);
/*
This frees the memory associated with uarray2. However, it does not free all 
the elements in the array. They have to be freed separately by applying a free
function using one of the mapping functions. It is a checked run-time error for
uarray2 or *uarray2 to be null.
*/

extern int UArray2_width( T uarray2);
/*
This returns the width (which is the number of columns) of uarray2.
*/

extern int UArray2_height( T uarray2);
/*
This returns the height (which is the number of rows) of uarray2.
*/

extern int UArray2_size(T uarray2);
/*
This returns the size of each element in uarray2.
*/

extern void *UArray2_at(T uarray2, int column, int row);
/*
This function returns a pointer to element stored at the location [column][row] 
in uarray2.
Invalid input is a checked run-time error.
*/

extern void UArray2_map_row_major(T uarray2, void apply(int width, int height, 
                                  T uarray2, void * elem, void * cl), void *
cl);
/*
This function calls the apply function on each element of uarray2 in row major 
order. This means column indices vary more rapdly than row indices.
*/

extern void UArray2_map_col_major(T uarray2, void apply(int width, int height, 
                                  T uarray2, void * elem, void * cl), void *
cl);
/*
This function calls the apply function on each element of uarray2 in column 
major order. This means row indices vary more rapdly than column indices.
*/

#undef T
#endif
