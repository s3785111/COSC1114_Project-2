#include "bestfit.h"
#include "libmem.h"

void *strategy(size_t chunk_size) {
  struct entry *entry   = NULL;
  struct entry *bestfit = NULL;

  while ((entry = (_getNextEntry(FREE, entry)))) {
    // Assign first entry that fits chunk if nothing yet found
    if (bestfit == NULL && entry->size >= chunk_size)
      bestfit = entry;
    // Otherwise, assign current best fit
    else if (entry->size >= chunk_size && entry->size < bestfit->size)
      bestfit = entry;
  }

  return bestfit;
}

int main(int argc, char *argv[]) {

  // Validate argument count
  if (argc != 2) {
    fprintf(stderr, "Fatal Error: Received %d input arguments, expected 2.\n", argc);
    return EXIT_FAILURE;
  }

  // Attempt to open input file
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Fatal Error: Input file %s does not exist.\n", argv[1]);
    return EXIT_FAILURE;
  }

  // Create chunk LIFO
  struct slisthead allocated_chunks = SLIST_HEAD_INITIALIZER(allocated_chunks);

  char line[50]; // No way test inputs should extend beyond this
  struct entry *entry;

  while (fgets(line, sizeof(line), fp)) {
    // [--------------------------------- ALLOC -----------------------------------]
    if (strncmp(line, "alloc:", 6) == 0) {
      // Allocate specified chunk size
      int size;
      sscanf(line + 6, "%d", &size);
      void *chunk = alloc(size);

      // Insert allocated chunk into LIFO
      // (reusing mem buffer entry struct because lazy, does not need to be pretty)
      entry       = malloc(sizeof(struct entry));
      entry->data = chunk;
      SLIST_INSERT_HEAD(&allocated_chunks, entry, entries);
    }
    // [--------------------------------- DEALLOC ---------------------------------]
    else if (strncmp(line, "dealloc", 7) == 0) {
      // Exit program if attempting to deallocate invalid address
      //
      // This validation occurs in the main loop as the node needs to be dereferenced
      // in order to pass the previously allocated chunk address to ```dealloc```.
      // Additional validation is performed by the allocator library to ensure the address
      // it is passed is valid before attempting to deallocate.
      if (!(entry = SLIST_FIRST(&allocated_chunks))) {
        fprintf(stderr, "Fatal Error: Attempting to deallocate invalid address.\n");
        return EXIT_FAILURE;
      }
      // Deallocate chunk, remove entry from list and free associated memory
      dealloc(entry->data);                          // Deallocate chunk stored in LIFO head
      SLIST_REMOVE_HEAD(&allocated_chunks, entries); // Remove entry from LIFO
      free(entry);                                   // Free memory removed from LIFO
    }
  }

  // Free any remaining nodes
  while (!SLIST_EMPTY(&allocated_chunks)) {
    entry = SLIST_FIRST(&allocated_chunks);
    SLIST_REMOVE_HEAD(&allocated_chunks, entries);
    free(entry);
  }

  printBlks(ALLOCATED);
  printBlks(FREE);

  return EXIT_SUCCESS;
}
