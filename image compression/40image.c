#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "pnm.h"
#include "math.h"
#include "assert.h"
#include "bitpack.h"

static void (*compress_or_decompress)(FILE *input) = compress40;

int main(int argc, char *argv[]) {
  int i;
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-c")) {
      compress_or_decompress = compress40;
    } else if (!strcmp(argv[i], "-d")) {
      compress_or_decompress = decompress40;
    } else if (*argv[i] == '-') {
      fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
      exit(1);
    } else if (argc - i > 2) {
      fprintf(stderr, "Usage: %s -d [filename]\n"
                      "       %s -c [filename]\n", argv[0], argv[0]);
      exit(1);
    } else {
      break;
    }
  }
  assert(argc - i <= 1); // at most one file on command line
  if (i < argc) {
    FILE* fp = fopen(argv[i], "r");
    assert(fp);
    compress_or_decompress(fp);
    fclose(fp);
  } else {
    compress_or_decompress(stdin);
  }

	uint64_t x = 31;
	unsigned width = 5;
	int64_t y = -14;
if(Bitpack_fitsu(x, width)){
	fprintf(stderr, "it worked\n");
}
if(Bitpack_fitss(y, width)){
	fprintf(stderr, "it worked\n");
}
uint64_t z = (100 << 4);
x = Bitpack_getu(z, 3, 4);
fprintf(stderr, "x = %u", (unsigned) x);

    return 0;

}
