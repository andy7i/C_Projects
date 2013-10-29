// bitpack.c
#include "bitpack.h"
#include <stdio.h>
#include "except.h"
#include "assert.h"

bool Bitpack_fitsu(uint64_t n, unsigned width){
	assert( width > 0);
	uint64_t check = 1;
	for(int i = width-1; i > 0; i--){
		check += (1 << i);
	}
	if(n > check)
		return false;
	return true;
}

bool Bitpack_fitss(int64_t n, unsigned width){
	assert( width > 0);
	uint64_t abs_n = ~n + 1;
	return Bitpack_fitsu(abs_n, width-1);
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb){
	assert(width > 0);
	int value = 0;
	int last_bit = lsb;
	for(int i = width+lsb-1; i >= last_bit; i--){
		value += (((1 << i) & word) >> lsb);
	}
	return value;
	
	
(void) word;
(void) width;
(void) lsb;
return 0;
}

 int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb){
(void) word;
(void) width;
(void) lsb;
return 0;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value){
(void) word;
(void) width;
(void) lsb;
(void) value;
return 0;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value){
(void) word;
(void) width;
(void) lsb;
(void) value;
return 0;
}
