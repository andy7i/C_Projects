#ifndef UMSECTIONS_INCLUDED
#define UMSECTIONS_INCLUDED
/* Full documentation for this interface is at http://tinyurl.com/2uwhhtu */
#include <stdint.h>
#include <stdio.h>
//#include <stdlib.h>
#include <seq.h>
#include <table.h>
#include <atom.h>
#include <mem.h>
#include <malloc.h>
#include <bitpack.h>
#define T Umsections_T
const int TABLE_HINT = 500;
const int SEQ_HINT = 50;
void applyFree(const void *key, void **value, void *cl);
void applyWrite(const void *key, void **value, void *cl);


//    The sequence of sections is represented as a table. Every section is a
// sequence of instruction words. Sections are referenced in the table using 
// Atoms as keys. Pointers to the sections are held as the values of the table.
//    We implemented an additional sequence that keeps track of the sequential
// order of sections (in chronological order). This was done so that the output
// of the write function would print sections in order.
//    Invariants are: 
//      1) There will never be two sections with the same name
//      2) The chornological order of sections is always stored
//      3) Instructions are always added to high and pushed from low

struct T {
    Table_T sections;
    Seq_T currentSection;
    int (*error)(void *errstate, const char *message);
    void* errstate;
    Seq_T sectionOrder;
};
typedef struct T *T;

T Umsections_new (const char *section, 
            int (*error)(void *errstate, const char *message),
            void *errstate) {
   T Umsections = malloc(sizeof(*Umsections));
   Umsections->sections = Table_new(TABLE_HINT, NULL, NULL);
   Seq_T temp;
   temp = Seq_new(SEQ_HINT);
   Seq_T temp2;
   temp2 = Seq_new(SEQ_HINT);

   Umsections->error = error;
   Umsections->errstate = errstate;

   Umsections->currentSection = temp;
   Umsections->sectionOrder = temp2;
   Seq_addlo(Umsections->sectionOrder, temp);
   Table_put(Umsections->sections, Atom_string(section), temp);  
/*    Table_map((Umsections)->sections,applyFree,NULL);
    Table_free(&((Umsections)->sections));
    Seq_free(&((Umsections)->sectionOrder));
//    Seq_free(((Umsectinos)->currentSection));
    free(Umsections);
    exit(1);*/
   return Umsections;
}

void Umsections_free(T *asmp) {
    Table_map((*asmp)->sections,applyFree,NULL);
    Table_free(&((*asmp)->sections));
    Seq_free(&((*asmp)->sectionOrder));
//    Seq_free(((*asmp)->currentSection));
    free(*asmp);
}
void applyFree(const void *key,void **value, void *cl){
  (void) key;
  (void) cl;
  Seq_free((((Seq_T*)value)));
}

int Umsections_error(T asm, const char *msg){
  return asm->error(asm->errstate, msg);
}

void Umsections_section(T asm, const char *section) {
  const char* atom;
  atom = Atom_string(section);  
  Seq_T temp = Table_get(asm->sections,atom);
  if (temp == NULL){
    temp = Seq_new(SEQ_HINT);
    Table_put(asm->sections, atom, temp);
    Seq_addhi(asm->sectionOrder, temp);
  }
  asm->currentSection = temp;
}

typedef uint32_t Umsections_word; // Universal Machine word
void Umsections_emit_word(T asm, Umsections_word data) {
  Seq_addhi(asm->currentSection, (void*)(uintptr_t)data);
}

void Umsections_map(T asm, void apply(const char *name, void *cl), void *cl){
  uint32_t length = Seq_length(asm->sectionOrder);
  length = length * 2;
  const char* temp = NULL;
  void **table = Table_toArray(asm->sections, NULL);
  for(uint32_t i=0; i < length; i += 2){    // increments by 2 since keys
    temp = table[i];                        // are always the even indexes
    apply(temp,cl);
  }
  free(table);
}

int Umsections_length(T asm, const char *name){
  const char* atom = Atom_string(name);
  Seq_T temp = Table_get(asm->sections, atom);
  if (temp == NULL)
      Umsections_error(asm, "Umsections_length out of bounds");
   return Seq_length(temp);
}
Umsections_word Umsections_getword(T asm, const char *name, int i){
 const char* atom = Atom_string(name);
 Seq_T temp = Table_get(asm->sections, atom);
 if (Seq_length(temp) <= i || i < 0)
      Umsections_error(asm, "Umsections_word out of bounds");
 return (uintptr_t)Seq_get(temp, i);
}

void Umsections_putword(T asm, const char *name, int i, Umsections_word w){
 const char* atom = Atom_string(name);
 Seq_T temp = Table_get(asm->sections, atom);
 if (temp == NULL)
      Umsections_error(asm, "Umsections_putword out of bounds");
 Seq_put(temp, i, (void*)(uintptr_t)w);
}

void Umsections_write(T asm, FILE *output){
    Seq_T current = NULL;
    for(int i = 0; i<Seq_length(asm->sectionOrder);i++){
      current = Seq_get(asm->sectionOrder,i);
      int length = Seq_length(current);
      for(int j=0; j<length;j++){
        uint32_t word = (uintptr_t)Seq_get(current, j);
        for(int k = 3; k > -1; k--)         // shorter way to print out 32/8 bit
            putc(Bitpack_gets(word, 8, 8*k), output);               // words
(void) word;
(void) output;
      }
    }
}



/* A value of type T represents a nonempty *sequence* of named sections.
Each section contains a sequence of Universal Machnine words.
In addition, each value of type T identifies one section in
the sequence as the ’current’ section, and a value of type T
encapsulates an error function.
*/
/* Callers of procedures in this interface need *not* provide
long-lived pointers to strings. Callers may reuse string memory
as soon as the call returns. */
#undef T
#endif

