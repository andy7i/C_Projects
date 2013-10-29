#ifndef UMOP_C
#define UMOP_C

#include "umop.h"
#include "ummem.c"

uint32_t reg_array [] = {0, 0, 0, 0, 0, 0, 0, 0};
int A, B, C;

void read_instruction(uint32_t instruction){
    int operation = (int)Bitpack_getu((uint64_t) instruction, 4, 28);
    switch (operation){
        case 0:
             A = (int)Bitpack_getu((uint64_t) instruction, 3, 6);
             B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
             C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            cond_mov();
            break;
        case 1:
            A = (int)Bitpack_getu((uint64_t) instruction, 3, 6);
            B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            seg_load();
            break;
        case 2:
            A = (int)Bitpack_getu((uint64_t) instruction, 3, 6);
            B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            seg_store();
            break;
        case 3:
            A = (int)Bitpack_getu((uint64_t) instruction, 3, 6);
            B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            add();
            break;
        case 4:
            A = (int)Bitpack_getu((uint64_t) instruction, 3, 6);
            B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            mult();
            break;
        case 5:
            A = (int)Bitpack_getu((uint64_t) instruction, 3, 6);
            B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            divide();
            break;
        case 6:
            A = (int)Bitpack_getu((uint64_t) instruction, 3, 6);
            B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            bit_nand();
            break;
        case 7:
            halt();
            break;
        case 8:
            B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            map_seg();
            break;
        case 9:
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            unmap_seg();
            break;
        case 10:
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            output();
            break;
        case 11:
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            input();
            break;
        case 12:
            B = (int)Bitpack_getu((uint64_t) instruction, 3, 3);
            C = (int)Bitpack_getu((uint64_t) instruction, 3, 0);
            load_prog();
            break;
        case 13:
            load_value(instruction);
            break;
        default:
            fprintf(stderr, "Something went wrong");
            break;
    }
}
//Might not need break statements

void cond_mov(){
    
    if(reg_array[C] != 0){
        reg_array[A] = reg_array[B];
    }
}
void seg_load(){
    //Check that not storing in 0
    reg_array[A] = load_word(reg_array[B], (int)reg_array[C]);
}
void seg_store(){
    store_word(reg_array[A], (int)reg_array[B], reg_array[C]);
}
void add(){
    reg_array[A] = (reg_array[B] + reg_array[C]);
}
void mult(){
    reg_array[A] = (reg_array[B] * reg_array[C]);
}
void divide(){
    reg_array[A] = (reg_array[B] / reg_array[C]);
}
void bit_nand(){
    reg_array[A] = (~(reg_array[B] & reg_array[C]));
}
void halt(){
    halt_program();
}
void map_seg(){
    int Seg_ID = create_segID();
    create_segment(reg_array[C], Seg_ID);
    reg_array[B] = Seg_ID;
}
void unmap_seg(){
    free_segment(reg_array[C]);
}
void output(){
    if(reg_array[C] < 256)
        putc(reg_array[C], stdout);
}
void input(){
    reg_array[C] = getc(stdin);
}
void load_prog(){
     load_program(reg_array[B], (int)reg_array[C]);
}
void load_value(uint32_t instruction){
    int A = (int)Bitpack_getu((uint64_t) instruction, 3, 25);
    reg_array[A] = (uint32_t)Bitpack_getu((uint64_t) instruction, 25, 0);
}

#endif

