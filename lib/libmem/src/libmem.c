#include "libmem.h"
#include <stdio.h>
#include <unistd.h>

static struct slisthead allocatedHead = SLIST_HEAD_INITIALIZER(allocatedHead);
static struct slisthead freedHead     = SLIST_HEAD_INITIALIZER(freeHead);

/* ------------------------------------- UTILITY -----------------------------------*/

/* =============================================================================== */
/**
 * @brief Prints the details of a single memory block.
 *
 * Displays the size, block address, and entry pointer for the specified memory
 * block, indicating whether it is allocated or deallocated.
 *
 * @param status Specifies whether the block is allocated or deallocated.
 *               Pass ALLOCATED for allocated blocks, and DEALLOCATED for freed blocks.
 * @param np Pointer to the memory block entry to be printed.
 *
 * =============================================================================== */
void printBlk(AllocStatus status, struct entry *np) {
  printf(status == ALLOCATED ? "[Alloc]" : "[Dealloc]");
  printf(" Size: %lu, Block: %p, Entry Pointer: %p\n", np->data.size, np->data.block, np);
}

/* =============================================================================== */
/**
 * @brief Prints the blocks in the allocated or freed memory list.
 *
 * Iterates through the specified memory list (allocated or freed) and prints
 * the size, block address, and entry pointer for each memory block.
 *
 * @param status Specifies whether to print the allocated or freed memory blocks.
 *               Pass ALLOCATED to print allocated blocks, and DEALLOCATED to
 *               print freed blocks.
 *
 * =============================================================================== */
void printBlks(AllocStatus status) {
  struct slisthead head = (status == ALLOCATED) ? allocatedHead : freedHead;
  struct entry *np;

  SLIST_FOREACH(np, &head, entries) {
    printBlk(status, np);
  }
}

/* ----------------------------------- MANAGEMENT ---------------------------------*/

/* =============================================================================== */
/**
 * @brief Allocates a block of memory.
 *
 * Allocates a memory block of the specified size, using `sbrk` to manage memory and
 * inserts the block into a linked list of memory block entries.
 *
 * @param chunk_size Size of the memory block to allocate.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 *
 * =============================================================================== */
void *alloc(size_t chunk_size) {
  printf("Calling alloc with size: %zu.\n", chunk_size);

  // Request memory for a new list entry
  // @todo First check free list if block is available to avoid unnecessary memory allocation
  // @todo Implement allocation strategy as macro defined by API consumer
  //
  // Considering this is a toy implementation of malloc i'm assuming this dynamic allocation
  // is okay and not considered a part of the memory "allocated" to the program. Other
  // implementations of linked lists would use dynamic allocation through malloc (C) or the
  // "new" keyword for a list class (C++) regardless.
  struct entry *newmem = (struct entry *)sbrk(sizeof(struct entry));
  if (newmem == (void *)-1) {
    fprintf(stderr, "Error: Failed to allocate memory for entry\n");
    return NULL;
  }

  // Allocate the requested block of memory
  newmem->data.block = sbrk(chunk_size);
  if (newmem->data.block == (void *)-1) {
    fprintf(stderr, "Error: Failed to allocate memory with sbrk\n");
    return NULL;
  }

  newmem->data.size = chunk_size;

  // Insert the new entry into the allocated list
  SLIST_INSERT_HEAD(&allocatedHead, newmem, entries);

  // Return the allocated block
  return newmem->data.block;
}

/* =============================================================================== */
/**
 * @brief Deallocates a block of memory.
 *
 * Deallocates the head of the allocated memory list if NULL is passed,
 * or returns an error if there is no allocated memory to free.
 *
 * @param chunk Pointer to the memory block to deallocate. If NULL, the head
 *              of the allocated list is deallocated.
 *
 * =============================================================================== */
void dealloc(void *chunk) {
  printf("Calling dealloc.\n");

  // Early exit with error if allocated list is empty
  if (SLIST_EMPTY(&allocatedHead)) {
    printf("Error: No allocated memory to free\n");
    return;
  }

  // Deallocate head if NULL is passed
  if (chunk == NULL) {
    // Remove block entry from allocated list
    struct entry *deallocmem = SLIST_FIRST(&allocatedHead);
    SLIST_REMOVE_HEAD(&allocatedHead, entries);

    // Insert block entry to head of freed list
    SLIST_INSERT_HEAD(&freedHead, deallocmem, entries);
  }
}
