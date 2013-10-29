#include "bitpack.h"
#include <stdio.h>
#include "except.h"
#include "assert.h"
#include "math.h"
Except_T Bitpack_Overflow = {"Overflow packing bits"};

bool Bitpack_fitsu(uint64_t n, unsigned width){
    assert(width <= 64);
    if(width == 0 && n == 0)
        return true;        //technically true
    if(width == 0)
       return false;
    uint64_t check = pow(2, width) - 1;
    if(n > check)
        return false;
    return true;
}

bool Bitpack_fitss(int64_t n, unsigned width){
    assert( width <= 64);
    uint64_t abs_n = (uint64_t) n;
    if(n < 0)
       abs_n = ~n + ((uint64_t) 1);
    return Bitpack_fitsu(abs_n, width-1);
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb){
    assert((width + lsb) <= 64);
    uint64_t value = 0;
    uint64_t temp = (pow(2,width)-1);
    if(lsb != 64)
        temp = temp << lsb;
    value = (temp & word) >> lsb;
    return value;
}

 int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb){
   assert((width + lsb) <= 64);
   uint64_t check = (((uint64_t)1 << (width+lsb-1)) & word);  
   int sign = 0;
   if(check > 0)
     sign = 1; 
   return (Bitpack_getu(word, width, lsb) - sign * pow(2, width));    
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                    uint64_t value){
   assert((width+lsb) < 64);
   if(!Bitpack_fitsu(value, width))
        RAISE(Bitpack_Overflow); 
   uint64_t temp = pow(2, width) - 1;
   temp = ~(temp << (lsb));
   word = (word & temp )| (value << (lsb));
   return word;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
         int64_t value){
   if(!Bitpack_fitss(value, width))
        RAISE(Bitpack_Overflow); 
   assert(Bitpack_fitss(value, width) && 0 < lsb && (width+lsb) < 64);
   uint64_t placeholders = pow(2, width) - 1;
   value = value & placeholders;    
   return (Bitpack_newu(word, width, lsb, value));
}


















