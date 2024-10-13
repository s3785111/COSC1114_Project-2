#include "firstfit.h"
#include "libmem.h"

void *strategy(size_t chunk_size) {
  struct entry *entry = NULL;

  while ((entry = (_getNextEntry(FREE, entry)))) {
    if (entry->size >= chunk_size)
      return entry;
  }

  return NULL;
}

int main(int argc, char *argv[]) {

  // Validate argument count
  if (argc != 2) {
    fprintf(stderr, "Fatal Error: Received %d input arguments, expected 1.", argc);
    return EXIT_FAILURE;
  }

  // Attempt to open input file
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Fatal Error: Input file %s does not exist.", argv[1]);
    return EXIT_FAILURE;
  }

  struct slisthead allocated_chunks = SLIST_HEAD_INITIALIZER(allocated_chunks); // Create chunk LIFO buffer
  char line[50];                                                                // No way test inputs should extend beyond this

  while (fgets(line, sizeof(line), fp)) {

    // ALLOC -----------------------------------
    if (strncmp(line, "alloc:", 6) == 0) {

      // Allocate specified chunk size
      int size;
      sscanf(line + 6, "%d", &size);
      void *chunk = alloc(size);

      // Insert allocated chunk into LIFO
      // (reusing mem buffer entry struct definition because lazy, does not need to be pretty)
      struct entry *entry = malloc(sizeof(struct entry));
      entry->data         = chunk;
      SLIST_INSERT_HEAD(&allocated_chunks, entry, entries);

    }

    // DEALLOC ---------------------------------
    else if (strncmp(line, "dealloc", 7) == 0) {
      struct entry *head = SLIST_FIRST(&allocated_chunks);
      SLIST_REMOVE_HEAD(&allocated_chunks, entries); // Remove entry from LIFO
      dealloc(head->data);                           // Deallocate chunk stored in LIFO head

      free(head);
    }
  }

  printBlks(ALLOCATED);
  printBlks(FREE);

  return EXIT_SUCCESS;
}
