#include "firstfit.h"
#include "libmem.h"

void *strategy(size_t chunk_size) {
  printf("Allocating with strategy: best fit.\n");

  struct entry *entry = NULL;

  while ((entry = (_getNextEntry(FREE, entry)))) {
    if (entry->size >= chunk_size)
      return entry;
  }

  return NULL;
}

int main() {
  void *alloc5  = alloc(5);
  void *alloc33 = alloc(33);

  dealloc(NULL);
  dealloc(NULL);

  void *alloc12 = alloc(12);

  dealloc(alloc12);

  printBlks(ALLOCATED);
  printBlks(FREE);

  return EXIT_SUCCESS;
}
