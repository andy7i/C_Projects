#include "stdlib.h"
#include "stdio.h"
#include "seq.h"
#include "bitpack.c"

static inline void emit(Seq_T instruction_stream, uint32_t instruction);
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
void output_instructions(Seq_T stream);
void emit_cond_mov_test(Seq_T stream);
void emit_seg_load_check(Seq_T stream);
void emit_halt_test(Seq_T stream);
void emit_cond_mov_test(Seq_T stream);
void emit_map_seg_test(Seq_T stream);
void emit_store_seg_test(Seq_T stream);
void emit_load_seg_test(Seq_T stream);
void emit_add_test(Seq_T stream);
void emit_mult_test(Seq_T stream);
void emit_div_test(Seq_T stream);
void emit_nand_test(Seq_T stream);
void emit_unmap_test(Seq_T stream);
void emit_input_test(Seq_T stream);
void emit_output_test(Seq_T stream);
void emit_load_value_test(Seq_T stream);
void emit_load_program_test(Seq_T stream);

void emit_store_seg_large_test(Seq_T stream);

int main(){
    Seq_T instruction_stream = Seq_new(1000);
    //emit_halt_test(instruction_stream);      //works
    emit_cond_mov_test(instruction_stream);  //works
    emit_map_seg_test(instruction_stream);   //works
    emit_store_seg_test(instruction_stream); //works
    emit_load_seg_test(instruction_stream);  //works
    emit_add_test(instruction_stream);       //works
    emit_mult_test(instruction_stream);      //works
    emit_div_test(instruction_stream);       //works
    emit_nand_test(instruction_stream);      //works but should check with all 1 values
    emit_unmap_test(instruction_stream);     //works
    emit_input_test(instruction_stream);     
    //emit_output_test(instruction_stream);    
    //emit_load_value_test(instruction_stream);//works
    emit_load_program_test(instruction_stream);
emit_store_seg_large_test(instruction_stream);

    output_instructions(instruction_stream);
    return 0;
}

void emit_load_program_test(Seq_T stream){
    //mult test
    emit(stream, load_value(0, 5));
    emit(stream, load_value(1, 10));
    emit(stream, mult(2, 1, 0));
    emit(stream, output(2));
    emit(stream, output(1));
    emit(stream, output(0));

    //begin loading
    emit(stream, load_value(0, 60));
    emit(stream, map_seg(2, 0));
    for (int i = 0; i < 6; i++){
        emit(stream, load_value(7, i));
        emit(stream, load_value(1, 0));
        emit(stream, seg_load(4, 1, 7));
        emit(stream, seg_store(2, 7, 4));
    }
    
    emit(stream, load_value(6, 0));
    emit(stream, load_prog(2, 6));
    emit(stream, halt());
}

void emit_load_seg_test(Seq_T stream){
    emit(stream, load_value(0, 10));
    emit(stream, load_value(1, 1));
    emit(stream, map_seg(2, 0)); //Creates segment with 10 words
    emit(stream, seg_store(2, 1, 0)); //Stores 10 in word 1 at the segment
    emit(stream, seg_load(3, 2, 1));
    emit(stream, output(3));
}
void emit_store_seg_test(Seq_T stream){
    emit(stream, load_value(0, 10));
    emit(stream, load_value(1, 1));
    emit(stream, map_seg(2, 0)); //Creates segment with 10 words
    emit(stream, seg_store(2, 1, 0)); //Stores 10 in word 1 at the segment
}
void emit_input_test(Seq_T stream){
    emit(stream, input(0));
    emit(stream, output(0));
}

void emit_mult_test(Seq_T stream){
    emit(stream, load_value(0, 5));
    emit(stream, load_value(1, 10));
    emit(stream, mult(2, 1, 0));
    emit(stream, output(2));
}

void emit_div_test(Seq_T stream){
    emit(stream, load_value(0, 10));
    emit(stream, load_value(1, 50));
    emit(stream, divide(2, 1, 0));
    emit(stream, output(2));
}

void emit_add_test(Seq_T stream){
    emit(stream, load_value(0, 33554431));
    emit(stream, load_value(1, 2));
    for(int i = 0; i < 130; i++)
         emit(stream, add(2, 2, 0));
    emit(stream, output(2));
}

void emit_nand_test(Seq_T stream){
    emit(stream, load_value(0, 0x1));
    emit(stream, load_value(1, 0x1));
    emit(stream, bit_nand(2, 1, 0));
    emit(stream, output(2));
}
void emit_unmap_test(Seq_T stream){
  emit(stream, load_value(0, 1));
  emit(stream, map_seg(1, 0));
  emit(stream, map_seg(2, 0));
  emit(stream, map_seg(3, 0));
  emit(stream, unmap_seg(1));
  emit(stream, unmap_seg(2));
  emit(stream, unmap_seg(3));
}
void emit_map_seg_test(Seq_T stream){
    emit(stream, load_value(0, 10));
    emit(stream, map_seg(3, 0));
    emit(stream, map_seg(2, 0));
    emit(stream, map_seg(1, 0));
    emit(stream, output(1));
    emit(stream, output(2));
    emit(stream, output(3));
}

void emit_cond_mov_test(Seq_T stream){
    emit(stream, load_value(0, 'A'));
    emit(stream, load_value(1, 'B'));
    emit(stream, cond_mov(2, 1, 0));
    emit(stream, output(2));
}

void emit_halt_test(Seq_T stream){
    emit(stream, halt());
    emit(stream, load_value(1, 'B'));
    emit(stream, output(1));
    emit(stream, load_value(1, 'a'));
    emit(stream, output(1));
    emit(stream, load_value(1, 'd'));
    emit(stream, output(1));
    emit(stream, load_value(1, '!'));
    emit(stream, output(1));
    emit(stream, load_value(1, '\n'));
    emit(stream, output(1));
}

void emit_store_seg_large_test(Seq_T stream){
    emit(stream, load_value(0, 100030));
    emit(stream, load_value(1, 1));
    emit(stream, map_seg(2, 0)); //Creates segment with 10002 words
    emit(stream, load_value(0, 1));
    emit(stream, seg_store(2, 1, 0)); //Stores 10 in word 1 at the segment
    for (int i = 0; i < 100020; i++){
        emit(stream, load_value(0, i)); 
        emit(stream, seg_store(2, 0, 0));
        emit(stream, seg_load(3, 2, 0));
        emit(stream, output(3));
    }
}

static inline void emit(Seq_T instruction_stream, uint32_t instruction){
     assert(sizeof(instruction) <= sizeof(uintptr_t));
     Seq_addhi(instruction_stream, (void*)(uintptr_t)instruction);
}
static inline uint64_t set_registers(int A, int B, int C){
    uint64_t instruction = 0;
    instruction = Bitpack_newu(instruction, 3, 0, (uint64_t)C);
    instruction = Bitpack_newu(instruction, 3, 3, (uint64_t)B);
    instruction = Bitpack_newu(instruction, 3, 6, (uint64_t)A);
    return instruction;
}
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
    uint64_t instruction = 0;
    instruction = (uint64_t) value;
    instruction = Bitpack_newu(instruction, 3, 25, reg);
    instruction = Bitpack_newu(instruction, 4, 28, 13);
    return instruction;
}

void output_instructions(Seq_T instruction_stream){
    uint32_t instruction;
    while(Seq_length(instruction_stream)!= 0){
        instruction = (uint32_t)(uintptr_t)Seq_remlo(instruction_stream);
        for(int x = 3; x >= 0; x--)
            putchar(Bitpack_gets(instruction, 8, 8*x));
    }
}




