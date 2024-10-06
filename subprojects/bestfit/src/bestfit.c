#include "bestfit.h"
#include "libmem.h"

struct entry *strategy(size_t chunk_size) {
  printf("Allocating with strategy: best fit.\n");
  return NULL;
}

int main() {
  void *alloc5  = alloc(5);
  void *alloc33 = alloc(33);

  printBlks(ALLOCATED);
  printBlks(FREE);

  dealloc(NULL);

  printBlks(ALLOCATED);
  printBlks(FREE);

  return 0;
}
