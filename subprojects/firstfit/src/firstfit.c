#include "firstfit.h"
#include "libmem.h"

struct entry *strategy(size_t chunk_size) {
  printf("Allocating with strategy: first fit\n");
  return NULL;
}

int main() {
  alloc(14);
  return 0;
}
