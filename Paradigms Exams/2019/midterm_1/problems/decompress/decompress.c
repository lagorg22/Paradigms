#include "decompress.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

void Decompress(char** data) {
  assert(data != NULL);
  char* encoded = *data;
  char* ret = NULL;
  int ret_size = 0;
  for (int i = 0; encoded[i]; ) {
    int control = *(unsigned char*)(encoded + i);
    int n = control >> 4;
    int m = control & ((1 << 4) - 1);
    ret = realloc(ret, ret_size + n * m + 1);
    for (int j = 0; j < m; ++j) {
      void* pt = ret + ret_size + n * j;
      memcpy(pt, encoded + i + 1, n);
    }
    ret_size += n * m;
    ret[ret_size] = '\0';
    i += n + 1;
  }
  free(*data);
  *data = ret;
}
