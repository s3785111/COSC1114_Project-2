#include "bestfit.h"
#include "libmem.h"

void *strategy(size_t chunk_size) {
  printf("Allocating with strategy: best fit.\n");

  struct entry *entry   = NULL;
  struct entry *bestfit = NULL;

  while ((entry = (_getNextEntry(FREE, entry)))) {
    if (entry->size >= chunk_size && entry->size < bestfit->size)
      bestfit = entry;
  }

  return bestfit;
}

int main() {
  return EXIT_SUCCESS;
}
