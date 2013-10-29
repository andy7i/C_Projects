#ifndef UMMEM_C
#define UMMEM_C

#include "ummem.h"
#include "umop.c"
#include "mem.h"

UArray_T seg_array;
UArray_T Seg_Length_Array;
Seq_T Seq_freed;
int max_value, program_counter, program_size;

void set_variables(){
    seg_array = UArray_new(100, sizeof(uint64_t*));
    Seg_Length_Array = UArray_new(UArray_length(seg_array), sizeof(int));
    Seq_freed = Seq_new(100);
    max_value = 0;
    program_counter = 0;
}
void halt_program(){ 
    fprintf(stdout, "its halting");
    int length = UArray_length(seg_array);
    for(int i = 0; i < length; i++)
        if(*(int*)UArray_at(Seg_Length_Array, i)!= 0)
            free_segment(i);
            
    UArray_free(&Seg_Length_Array);
    Seq_free(&Seq_freed);
    UArray_free(&seg_array);
    exit(1);
}
void UM_run(FILE * input){
    set_variables();
    read_file(input);
    fprintf(stderr, "????\n");
    fclose(input);
    //Reads in the instructions and stores them into [0]. Probably need
    //to store them in temporary mem location.
    for(program_counter = 0; program_counter < program_size; program_counter++){
        read_instruction((uint32_t)load_word((uint32_t)0, program_counter));
    }
   halt_program();
}

void read_file(FILE * input){
    uint32_t newInstruction = 0;
    Seq_T instr_seq = Seq_new(100);
    int newChar = (int)getc(input);
    while(newChar != EOF){
        newInstruction = Bitpack_newu(newInstruction, 8, 24, newChar);
        for(int x = 2; x >= 0; x--){
            newInstruction = Bitpack_newu(newInstruction, 8, 8*x, getc(input));
        }    
        Seq_addhi(instr_seq, (void*)(uintptr_t)newInstruction);
        newChar = (int)getc(input);
    }
    create_segment(Seq_length(instr_seq), 0);
    program_size = Seq_length(instr_seq);
    for(int i = 0; Seq_length(instr_seq) != 0; i++)
        store_word(0, i, (uint32_t)(uintptr_t)Seq_remlo(instr_seq));
    Seq_free(&instr_seq);
}

void load_program(uint32_t Seg_ID, uint32_t word_offset){
    if(Seg_ID != 0){
        uint64_t** temp = UArray_at(seg_array, (int)Seg_ID);
        int new_prog_size = *(int*)UArray_at(Seg_Length_Array, Seg_ID);
        free_segment(0);
        create_segment(new_prog_size, 0);
        uint64_t** new_program = UArray_at(seg_array, 0);
        for(int i = 0; i < new_prog_size; i++)
            (*new_program)[i] = (*temp)[i];    
        program_size = new_prog_size;
    }
    program_counter = word_offset-1;
    return;
}

uint64_t load_word(uint32_t Seg_ID, uint32_t word_offset){
   // if(Seg_ID == 2)
    //fprintf(stderr, "segID = %u, word_offset = %u\n", Seg_ID, word_offset); 
   // if(word_offset > (unsigned)*(int*)UArray_at(Seg_Length_Array, Seg_ID)){
    //    fprintf(stderr, "bad call Seg_ID = %u, word_offset = %u\n", Seg_ID,
     //           word_offset);
    //char temp = getc(stdin);
    //(void) temp;
    //if (temp != temp)
    //    exit(1);
   // }
    uint64_t ** segment = UArray_at(seg_array, (int)Seg_ID);
    uint64_t word = (*segment)[word_offset];
    //if(word_offset > (unsigned)*(int*)UArray_at(Seg_Length_Array, Seg_ID))
    //fprintf(stderr, "asdsad\n");
    return word;
}
uint32_t create_segID(){
    uint32_t Seg_ID;
    if(Seq_length(Seq_freed) != 0){
        Seg_ID = (uint32_t)(uintptr_t)Seq_remlo(Seq_freed); //Might not need typecast.
    }
    else{
        max_value++;
        Seg_ID = (uint32_t)max_value; //Might need to set then incrememnt.
    }
    return Seg_ID;
}

//Might be able to merge these two functions

void create_segment(int num_words, uint32_t Seg_ID){
    //fprintf(stderr, "segid: %u, numwords: %u", Seg_ID, num_words);
    if(Seg_ID == (unsigned)UArray_length(seg_array)){
        UArray_resize(seg_array, Seg_ID*1.2); //Might want to change resized 
        UArray_resize(Seg_Length_Array, Seg_ID*1.2);
    }
    *(int*)UArray_at(Seg_Length_Array, Seg_ID) = num_words;
    uint64_t ** temp = (uint64_t**)UArray_at(seg_array, (int)Seg_ID);
    *temp = malloc(sizeof(uint64_t)*num_words);
    for(int i = 0; i < num_words; i++){
        //*temp[i] = (uint64_t)11;
        (*temp)[i] = 0;
//fprintf(stderr, "%d|", i);
}
    return;
}

void free_segment(uint32_t Seg_ID){
    uint64_t ** temp = (uint64_t**)UArray_at(seg_array, (int)Seg_ID);
    free(*temp);
    *(int*)UArray_at(Seg_Length_Array, Seg_ID) = 0;
    if(Seg_ID != 0)
    Seq_addhi(Seq_freed, (void *)(uintptr_t)Seg_ID); 
    return;
}

void store_word(uint32_t Seg_ID, uint32_t word_offset, uint32_t value){
    uint64_t ** segment = (uint64_t**)UArray_at(seg_array, (int)Seg_ID);
    (*segment)[word_offset] = value;
    return;
}


#endif
