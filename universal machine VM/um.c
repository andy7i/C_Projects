#include "set.h"
#include "uarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
//#include "bitpack.h"
#include "ummem.c"
#include "umop.c"
#include "assert.h"

void check_newfree();
void check_length();
void check_storeload();

int main(int argc, char* argv[]){
    
     /*if(argc != 2){
        fprintf(stderr, "Not correct number of files\n");
        exit(1);
    }
    */

    FILE * input = fopen(argv[1], "r");

     UM_run(input);
     (void) argc;
     (void) argv;
     return 0;
}

void check_newfree(){
    create_segment(1, 0x1);
    uint64_t * segment = UArray_at(seg_array, 1);
    assert(segment != NULL);
    free_segment(0x1);
}

void check_length(){
    create_segment(5, 0x1);
    int length = *(int*)UArray_at(Seg_Length_Array, 1);
    printf("%d\n", length);
    assert(length == 5);
}

void check_storeload(){
    create_segment(1, 0x1);
    create_segment(5, 0x2);
    create_segment(10, 0x3);
    store_word(0x1, 0, 0x1);
    store_word(0x2, 4, 0x10);
    store_word(0x3, 4, 0x100);
    uint64_t a = load_word(1, 0);
    uint64_t b = load_word(2, 4);
    uint64_t c = load_word(3, 4);
    assert(a == 0x1 && b == 0x10 && c == 0x100);
}

