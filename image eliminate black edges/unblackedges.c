#ifndef UNBLACKEDGES_C
#define UNBLACKEDGES_C
#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include <except.h>
#include <list.h>
#include <mem.h>
#include "bit2.h"
#include "seq.h"

void checkEdges(Bit2_T* blackedge);
void printPbm(Bit2_T blackedge);
void floodFill(Bit2_T blackedge, Seq_T* blackQueue, int i, int j);
void buildMap (Bit2_T blackedge, Pnmrdr_T image_map, Pnmrdr_mapdata mapdata);
Pnmrdr_T readGrid(FILE *input);
static int readInPixels (int elem, void* image_map, int row, int col, 
                              Bit2_T blackedge);
                              
//created for passing in information to the sequence/queue
struct index
{
      int row;
      int col;
};

typedef struct index dataindex;

int main(int argc, char *argv[])
{
    FILE *input = NULL;

    if (argc > 2)
    {
      fprintf(stderr, "Too many arguments.\n");
      return 1;
    }
    else if (argc == 2) 
      input = fopen(argv[1], "r");
    else if (argc == 1)
      input = stdin;
    if (input != NULL)
    {
        Pnmrdr_T image_map;
        image_map = readGrid(input);
        Pnmrdr_mapdata mapdata = Pnmrdr_data(image_map);
        Bit2_T blackedge = Bit2_new(mapdata.height,mapdata.width);
        buildMap(blackedge, image_map, mapdata);
        checkEdges(&blackedge);
        printPbm(blackedge);
        Bit2_free(&blackedge);
        fclose(input);
        free(image_map);
        fclose(input);
    }
    else fprintf(stderr, "File not opened succesfully\n");

    return 0;
}


Pnmrdr_T readGrid (FILE *input)
{
    int ret = 1;
    Pnmrdr_T image_map = NULL;
    
    TRY
      image_map = Pnmrdr_new(input);
      ret = 0;
    EXCEPT(Pnmrdr_Badformat)
      fprintf(stderr, "File not in pnm format.\n");
      ret = 1;
    EXCEPT(Pnmrdr_Count)
      fprintf(stderr, "File size is not correct.\n");
      ret = 1 ;
    ELSE
      fprintf(stderr, "Unknown exception raised.\n");
      fclose(input);
      ret = 1 ;
    END_TRY;
    if (ret == 1)
        exit(1);
    else return image_map;
}

// This function does a typecheck to make sure file is pnm
// It then reads the entire file, cleaning the edges as it goes
// printing the result will be handled in a seperate function
void buildMap(Bit2_T blackedge, Pnmrdr_T image_map, Pnmrdr_mapdata mapdata)
{
    if (mapdata.type != Pnmrdr_bit)
    {
      fprintf(stderr, "File not graymap.\n");
      free(image_map);
      exit(1);
      Bit2_free(&blackedge);
    }

    int (*pt2readInPixels)(int, void*, int, int, Bit2_T);
    pt2readInPixels = readInPixels;            
    Bit2_map_row_major(blackedge, pt2readInPixels, image_map);
}

// "apply" function for map_ func
int readInPixels (int elem, void* image_map, int row, int col, Bit2_T
      bit2)
{
    (void) row;
    (void) col;
    (void) bit2;
    (void) elem;
    int pixel = Pnmrdr_get((Pnmrdr_T)image_map);
    if (pixel != 0 && pixel != 1)
    {
        fprintf(stderr, "Pixel out of bounds. File not correct format\n");
        free(image_map);
        Bit2_free(&bit2);
        exit(1);
    }
    return pixel;
}
    
void checkEdges (Bit2_T* blackedge)
{
    Seq_T blackQueue = Seq_new(0);
    int j, i;
    
    for (i = 0; i < Bit2_height(*blackedge); i++)   //checks top/bottom rows
    {
        for (j = 0; j < Bit2_width(*blackedge); j++)
        {
            if (Bit2_get(*blackedge, i, j) == 1)
            {
                    Bit2_put(*blackedge, i, j, 0);
                    dataindex* data = malloc (sizeof(dataindex));
                    data->row = i;
                    data->col = j;
                    Seq_addhi(blackQueue, data);
            }
        }
        if(i == 0 && Bit2_height(*blackedge) != 1)  //swaps to end row
                i = Bit2_height(*blackedge)-2;
    }

    for(j = 0; j < Bit2_width(*blackedge); j++)     //checks left/right edges
    {
        for(i = 0; i < Bit2_height(*blackedge); i++)
        {
            if(Bit2_get(*blackedge, i, j) == 1)
            {
                Bit2_put(*blackedge, i, j, 0);
                dataindex* data = malloc (sizeof(dataindex));
                data->row = i;
                data->col = j;
                Seq_addhi(blackQueue, data);
            }
        }
        if (j == 0 && Bit2_width(*blackedge) != 1)  //swaps to the right edge
            j = Bit2_width(*blackedge)-2;
    }

    while(Seq_length(blackQueue) != 0)
    {
    dataindex* data  = Seq_remlo(blackQueue);
    floodFill(*blackedge, &blackQueue, (data->row),
        (data->col));
        FREE (data);
    }
    Seq_free(&blackQueue);
}

// this function checks around a given pixel to see if it is bordered
// by another black.  If so, it pushes the pointer to bit 
void floodFill(Bit2_T blackedge, Seq_T* blackQueue, int row, int col)
{
          Bit2_put(blackedge, row, col, 0);
          if(row - 1 >= 0 && Bit2_get(blackedge, row-1, col) == 1)
          {
                Bit2_put(blackedge, row-1, col, 0);
                dataindex* data = malloc (sizeof(dataindex));
                data->row = row-1;
                data->col = col;
                Seq_addhi(*blackQueue, data);
          }
          if(row + 1 < Bit2_height(blackedge) 
                        && Bit2_get(blackedge, row + 1, col) == 1)
          {
                Bit2_put(blackedge, row+1, col, 0);
                dataindex* data = malloc (sizeof(dataindex));
                data->row = row+1;
                data->col = col;
                Seq_addhi(*blackQueue, data);
          }
          if(col - 1 >= 0 && Bit2_get(blackedge, row, col-1) == 1)
          {
                Bit2_put(blackedge, row, col-1, 0);
                dataindex* data = malloc (sizeof(dataindex));
                data->row = row;
                data->col = col-1;
                Seq_addhi(*blackQueue, data);
          }

          if(col + 1 < Bit2_width(blackedge) 
                        && Bit2_get(blackedge, row, col + 1) == 1)
          {
                Bit2_put(blackedge, row, col+1, 0);
                dataindex* data = malloc (sizeof(dataindex));
                data->row = row;
                data->col = col+1;
                Seq_addhi(*blackQueue, data);
          }
}

void printPbm(Bit2_T blackedge)
{
    printf("P1\n%d %d\n", Bit2_width(blackedge), Bit2_height(blackedge));
      int i, j;
      j = 0;
      const char *prefix = "";
      const char *prefix2 = "";
      for(i = 0; i < Bit2_height(blackedge); i++)
//      for(i = 0; i <50 ; i++)
      {
          printf("%s", prefix2);
          for(j = 0; j < Bit2_width(blackedge); j++)
          {
              if (j % 35 == 0)
                  printf("\n");
                printf( "%s%d", prefix, Bit2_get(blackedge, i, j));
        prefix = " ";
          }
      prefix = "";
          prefix2 = "\n";
      }
      printf("\n");
}
#endif

