#ifndef UMOP_H
#define UMOP_H

#include "uarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "bitpack.c"

void read_instruction(uint32_t instruction);
//Read opcode value and call appropriate function from below

void cond_mov();
void seg_load();
void seg_store();
void add();
void mult();
void divide();
void bit_nand();
void halt();
void map_seg();
void unmap_seg();
void output();
void input();
void load_prog();
void load_value(uint32_t instruction);

#endif

