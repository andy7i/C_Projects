//ummacros.c
#include "ummacros.h"
#include <bitpack.h>
#include <stdio.h>
#include <stdlib.h> // for exit function

uint32_t cond_mov(int A, int B, int C);
uint32_t seg_load(int A, int B, int C);
uint32_t seg_store(int A, int B, int C);
uint32_t add(int A, int B, int C);
uint32_t mult(int A, int B, int C);
uint32_t divide(int A, int B, int C);
uint32_t bit_nand(int A, int B, int C);
uint32_t halt();
uint32_t map_seg(int B, int C);
uint32_t unmap_seg(int C);
uint32_t output(int C);
uint32_t input(int C);
uint32_t load_prog(int B, int C);
uint32_t load_value(int reg, uint32_t value);
void mov_macro(Umsections_T asm, Ummacros_Reg A, Ummacros_Reg B);
void bitwise_com_macro(Umsections_T asm, Ummacros_Reg A, Ummacros_Reg B);
void neg_macro(Umsections_T asm, int temporary, Ummacros_Reg A, Ummacros_Reg B);
void sub_macro(Umsections_T asm, int temporary, Ummacros_Reg A, Ummacros_Reg B,
        Ummacros_Reg C);
void and_macro(Umsections_T asm, int temporary, Ummacros_Reg A, Ummacros_Reg B,
        Ummacros_Reg C);
void or_macro(Umsections_T asm, int temporary, Ummacros_Reg A, Ummacros_Reg B,
        Ummacros_Reg C);

void Ummacros_op(Umsections_T asm, Ummacros_Op operator, int temporary,
                 Ummacros_Reg A, Ummacros_Reg B, Ummacros_Reg C){
  switch(operator){
      case MOV:
          mov_macro(asm, A, B);
          break;
      case COM:
          bitwise_com_macro(asm, A, B);
          break;
      case NEG:
          neg_macro(asm, temporary, A, B);
          break;
      case SUB:
          sub_macro(asm, temporary, A, B, C);
          break;
      case AND:
          and_macro(asm, temporary, A, B, C);
          break;
      case OR:
          or_macro(asm, temporary, A, B, C);
          break;
      default:
          break;
    }
}


// if number is less than 2^25-1, then it loads value directly
// else it does: k = (k/100000)*100000 + k%100000
void Ummacros_load_literal(Umsections_T asm, int temporary,
                        Ummacros_Reg A, uint32_t k){

    if (k <= 33554431){
        Umsections_emit_word(asm,load_value(A,k));
        return;
    }
    
    if (temporary == -1)
        Umsections_error(asm, "Load literal failed due to no temp");

  uint32_t upperHalf = k/100000;
  uint32_t lowerHalf = k%100000;
  Umsections_emit_word(asm,load_value(A,upperHalf));
  Umsections_emit_word(asm,load_value(temporary,100000));
  Umsections_emit_word(asm,mult(A,temporary,A));
  Umsections_emit_word(asm,load_value(temporary,lowerHalf));
  Umsections_emit_word(asm,add(A,A,temporary));
}

// The condition in condition_move will always evaluate as True, so it will
// always move.
void mov_macro(Umsections_T asm, Ummacros_Reg A, Ummacros_Reg B){
    uint32_t instruction = load_value(A, 1);
    uint32_t instruction2 = cond_mov(A, B, A);
    Umsections_emit_word(asm, instruction);
    Umsections_emit_word(asm, instruction2);
    
}

// ~(x) = ( nand(nand(0,0),x) )
void bitwise_com_macro(Umsections_T asm, Ummacros_Reg A, Ummacros_Reg B){
  Umsections_emit_word(asm,load_value(A, 0));
  Umsections_emit_word(asm,bit_nand(A, A, A));
  Umsections_emit_word(asm,bit_nand(A, A, B));
}

// -(x) = ~x + 1
void neg_macro(Umsections_T asm, int temporary, Ummacros_Reg A, Ummacros_Reg B){
    if (temporary == -1)
        Umsections_error(asm, "Neg failed due to no temp");
  bitwise_com_macro(asm, A, B);
  Umsections_emit_word(asm,load_value(temporary, 1));
  Umsections_emit_word(asm,add(A, A, temporary));
}

// x - y = x + -(y)
void sub_macro(Umsections_T asm, int temporary, Ummacros_Reg A, Ummacros_Reg B,
        Ummacros_Reg C){
    if (temporary == -1)
        Umsections_error(asm, "Sub failed due to no temp");
    neg_macro(asm, temporary, A, C);
    Umsections_emit_word(asm,add(A, B, A));
}

// x /\ y = ~(nand(x,y))
void and_macro(Umsections_T asm, int temporary, Ummacros_Reg A, Ummacros_Reg B,
        Ummacros_Reg C){
    if (temporary == -1)
        Umsections_error(asm, "And failed due to no temp");
  Umsections_emit_word(asm,bit_nand(temporary, B, C));
  bitwise_com_macro(asm, A, temporary);
}

// x \/ y = x + y - (x /\ y) 
void or_macro(Umsections_T asm, int temporary, Ummacros_Reg A, Ummacros_Reg B,
        Ummacros_Reg C){
    if (temporary == -1)
        Umsections_error(asm, "OR failed due to no temp");
  and_macro(asm, A, temporary, B, C);
  Umsections_emit_word(asm, add(B, B, C));
  sub_macro(asm, C, A, B, temporary);
}


static uint64_t set_registers(int A, int B, int C){
    uint64_t instruction = 0;
    instruction = Bitpack_newu(instruction, 3, 0, (uint64_t)C);
    instruction = Bitpack_newu(instruction, 3, 3, (uint64_t)B);
    instruction = Bitpack_newu(instruction, 3, 6, (uint64_t)A);
    return instruction;
}

// the rest of these are for the original 13 OPCODES
uint32_t cond_mov(int A, int B, int C){
    uint64_t instruction = set_registers(A, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 0);
}
uint32_t seg_load(int A, int B, int C){
    uint64_t instruction = set_registers(A, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 1);
}
uint32_t seg_store(int A, int B, int C){
    uint64_t instruction = set_registers(A, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 2);
}
uint32_t add(int A, int B, int C){
    uint64_t instruction = set_registers(A, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 3);
}
uint32_t mult(int A, int B, int C){
    uint64_t instruction = set_registers(A, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 4);
}
uint32_t divide(int A, int B, int C){
    uint64_t instruction = set_registers(A, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 5);
}
uint32_t bit_nand(int A, int B, int C){
    uint64_t instruction = set_registers(A, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 6);
}
uint32_t halt(){
    uint64_t instruction = 0;
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 7);
}
uint32_t map_seg(int B, int C){
    uint64_t instruction = set_registers(0, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 8);
}
uint32_t unmap_seg(int C){
    uint64_t instruction = set_registers(0, 0, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 9);
}
uint32_t output(int C){
    uint64_t instruction = set_registers(0, 0, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 10);
}
uint32_t input(int C){
    uint64_t instruction = set_registers(0, 0, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 11);
}
uint32_t load_prog(int B, int C){
    uint64_t instruction = set_registers(0, B, C);
    return (uint32_t)Bitpack_newu(instruction, 4, 28, 12);
}
uint32_t load_value(int reg, uint32_t value){
    uint32_t instruction = 0;
    instruction = value;
    instruction = Bitpack_newu(instruction, 4, 28, 13);
    instruction = Bitpack_newu(instruction, 3, 25, reg);
    return instruction;
}


