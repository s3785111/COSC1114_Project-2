#include "libmem.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static struct slisthead allocatedHead = SLIST_HEAD_INITIALIZER(allocatedHead);
static struct slisthead freedHead     = SLIST_HEAD_INITIALIZER(freeHead);

struct entry *strategy(size_t chunk_size) {
  fprintf(stderr, "Error: Allocation strategy not implemented\n");
  exit(EXIT_FAILURE);
}

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
  struct entry *allocatedchunk;

  // Normalise chunk size to nearest valid number
  size_t allocation_size;
  if (chunk_size <= 512) allocation_size = 512;
  if (chunk_size <= 256) allocation_size = 256;
  if (chunk_size <= 128) allocation_size = 128;
  if (chunk_size <= 64) allocation_size = 64;
  if (chunk_size <= 32) allocation_size = 32;

  // Attempt to allocate based on strategy
  if ((allocatedchunk = strategy(allocation_size)))
    return allocatedchunk;

  // Otherwise, allocate new memory block:

  // Request memory for a new list entry
  // @todo First check free list if block is available to avoid unnecessary memory allocation
  //
  // Considering this is a toy implementation of malloc, i'm assuming this dynamic allocation
  // is not considered a part of the memory "allocated" to the program. Other implementations
  // of linked lists would use dynamic allocation for "entries" or list nodes regardless.
  struct entry *newmem = (struct entry *)sbrk(sizeof(struct entry));
  if (newmem == (void *)-1) {
    fprintf(stderr, "Error: Failed to allocate memory for entry\n");
    exit(EXIT_FAILURE);
  }

  // Allocate the requested block of memory
  newmem->data.block = sbrk(allocation_size);
  if (newmem->data.block == (void *)-1) {
    fprintf(stderr, "Error: Failed to allocate memory with sbrk\n");
    exit(EXIT_FAILURE);
  }

  newmem->data.size = allocation_size;

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
    struct entry *deallocmem = SLIST_FIRST(&allocatedHead);
    SLIST_REMOVE_HEAD(&allocatedHead, entries);
    SLIST_INSERT_HEAD(&freedHead, deallocmem, entries);
  }

  // Otherwise, search for block to deallocate
  else {
    struct entry *deallocmem  = NULL;
    struct entry *searchedblk = NULL;

    // Traverse list for matching entry
    SLIST_FOREACH(deallocmem, &allocatedHead, entries) {
      if (deallocmem->data.block == chunk) {
        searchedblk = deallocmem;
        break;
      }
    }

    // Move block to deallocated list if matching entry is found
    if (searchedblk != NULL && searchedblk->data.block == chunk) {
      SLIST_REMOVE(&allocatedHead, searchedblk, entry, entries);
      SLIST_INSERT_HEAD(&freedHead, searchedblk, entries);
    }

    // Otherwise, exit with error code
    else {
      fprintf(stderr, "Fatal Error: Attempting to deallocate invalid block\n");
      fprintf(stderr, "Exiting.\n");
      exit(EXIT_FAILURE);
    }
  }
}
