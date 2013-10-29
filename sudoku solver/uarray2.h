// uarray2.h
// Written by James Downer and Andrew Jang
// Last edited September 17, 2012
#ifndef UARRAY2_T
#define UARRAY2_T
#include "uarray.h"

#define T UArray2_T
typedef struct T *T;


T UArray2_new(int height, int width, int size); 
//Create a 2D array of size width * height
                                 // and return a pointer to where it is
void UArray2_free(T *array2); //Return an array to memory
void* UArray2_at(T array2, int row, int col); //Return a pointer to an element 
                                           //to operate on
//void* UArray2_apply(T* array, void* func(), void* cl);
// used by map_col_major and 
                     //map_row_major to operate on elements
int UArray2_size(T array); //Returns max capacity of the array

//call the apply function on every element, traverse rows slowly
void UArray2_map_row_major(T array2, void apply(void* elem, void* cl, 
                int row, int col, T array), void* cl);
                
//call the apply function on every element, traverse cols slowly
void UArray2_map_col_major(T array2, void apply(void* elem, void* cl, 
                int row, int col, T array), void* cl);

#endif
