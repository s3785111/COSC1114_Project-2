#include "libmem.h"
#include <stdio.h>
#include <unistd.h>

static struct slisthead allocatedHead = SLIST_HEAD_INITIALIZER(allocatedHead);
static struct slisthead freedHead     = SLIST_HEAD_INITIALIZER(freeHead);

void *alloc(size_t chunk_size) {
  printf("Calling alloc with size: %zu.\n", chunk_size);

#ifndef ALLOC_STRATEGY
  fprintf(stderr, "Error: Failed to identify allocation strategy\n");
  return NULL;
#else
#endif

  // Request memory for a new list entry
  //
  // Considering this is a toy implementation of malloc i'm assuming this dynamic allocation
  // is okay and not considered a part of the memory "allocated" to the program. Other
  // implementations of linked lists would use dynamic allocation through malloc (C) or the
  // "new" keyword for a list class (C++) regardless.
  struct entry *newmem = (struct entry *) sbrk(sizeof(struct entry));
  if (newmem == (void *)-1) {
    fprintf(stderr, "Error: Failed to allocate memory for entry\n");
    return NULL;
  }

  // @todo Add logic to check free list for available block
  // implement allocation strategy as macro defined by API consumer

  // Allocate the requested block of memory
  newmem->data.block = sbrk(chunk_size);
  if (newmem->data.block == (void *)-1) {
    fprintf(stderr, "Error: Failed to allocate memory with sbrk\n");
    return NULL;
  }

  newmem->data.size = chunk_size;

  // Insert the new entry into the allocated list
  SLIST_INSERT_HEAD(&allocatedHead, newmem, entries);

  // Print all allocated entries
  struct entry *np;
  SLIST_FOREACH(np, &allocatedHead, entries) {
    printf("Size: %lu, Block: %p, Entry Pointer: %p\n", np->data.size, np->data.block, np);
  }

  // Return the allocated block
  return newmem->data.block;
}

void dealloc(void *chunk) {
  printf("Calling dealloc.\n");
}
