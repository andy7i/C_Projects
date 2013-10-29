// uarray2.h
// Written by James Downer and Andrew Jang
// Last edited September 17, 2012
#ifndef BIT2_T
#define BIT2_T
#include "uarray.h"
#include "bit.h"
#define R Bit2_T

typedef struct R *R;


R Bit2_new(int height, int width); //Create a 2D array of size width * height
                                 // and return a pointer to where it is
void Bit2_free(R *bit2); //Return an array to memory
int Bit2_size(R bit2); //Returns max capacity of the array

int Bit2_width(R bit2); //returns width
int Bit2_height(R bit2);    // returns height
int Bit2_area(R bit2);  //returns total area

int Bit2_get(R bit2, int row, int col); //Returns the value of a bit

int Bit2_put(R bit2, int row, int col, int bit); // adds input to an element in
                                                //    the 2D bit bit2

//call the apply function on every element, traverse rows slowly
void Bit2_map_row_major(R bit2, int apply(int elem, void* cl, 
                int row, int col, R bit2), void* cl);
                
//call the apply function on every element, traverse cols slowly
void Bit2_map_col_major(R bit2, int apply(int elem, void* cl, 
                int row, int col, R bit2), void* cl);

#endif

