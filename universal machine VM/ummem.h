#ifndef UMMEM_H
#define UMMEM_H

#include  "seq.h"
#include "uarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>

void UM_run(FILE* input);
//Takes input and runs the UM on that input command.
//Create seg_array (the Dynamic UArray containing the segments)

uint64_t load_word(uint32_t SegID, uint32_t word_offset);
//Gets the word at $m[SegID][word_offset]

uint32_t create_segID();
//This will return the next appropriate segment ID. That is, it will take an ID 
//from the sequence or if the sequence is empty it will return max_value + 1 and
//it will increment the value.

void create_segment(int num_words, uint32_t SegID);
//Called by the OPCODE map_seg function. It creates the segment in seg_array.

void free_segment(uint32_t SegID);
//Called by the OPCODE unmap_seg function. It frees the segment in seg_array. 

void store_word(uint32_t SegID, uint32_t word_offset, uint32_t value);
//Called by the OPCODE store function. It stores value into 
//$m[SegID][word_offset].

void read_file(FILE * input);

void set_variables();

void load_program(uint32_t Seg_ID, uint32_t word_offset);
void halt_program();
#endif

