#include "libmem.h"

static struct slisthead allocatedHead = SLIST_HEAD_INITIALIZER(allocatedHead);
static struct slisthead freedHead     = SLIST_HEAD_INITIALIZER(freeHead);

void *strategy(size_t __attribute__((unused)) chunk_size) {
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
  printf("Address: %p, Total Chunk Size: %lu, Allocated Chunk Size: %lu\n", np->data, np->size, np->allocated_size);
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

/* ------------------------------------- ACCESS -----------------------------------*/

/* =============================================================================== */
/**
 * @brief Retrieves the next entry in the chosen list.
 *
 * Returns the next entry in the allocated or freed list depending on the
 * specified status. If `entry` is NULL, returns the head of the list.
 *
 * @param status Specifies whether to search in the allocated or freed list.
 *               Pass ALLOCATED for the allocated list, or DEALLOCATED for the freed list.
 * @param entry  Pointer to the current entry. If NULL, returns the head of the list.
 *
 * @return Pointer to the next entry in the list, or NULL if at the end of the list.
 *
 * =============================================================================== */
struct entry *_getNextEntry(AllocStatus status, struct entry *entry) {
  struct slisthead head = (status == ALLOCATED) ? allocatedHead : freedHead;

  // Return head entry if passed null
  if (entry == NULL)
    return head.slh_first;

  // Otherwise, return next entry
  return SLIST_NEXT(entry, entries);
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
  struct entry *allocated_chunk;

  // Normalise chunk size to nearest valid number
  size_t allocation_size;
  if (chunk_size <= 512) allocation_size = 512;
  if (chunk_size <= 256) allocation_size = 256;
  if (chunk_size <= 128) allocation_size = 128;
  if (chunk_size <= 64) allocation_size = 64;
  if (chunk_size <= 32) allocation_size = 32;

  // Attempt to allocate based on strategy
  if ((allocated_chunk = strategy(allocation_size))) {
    SLIST_REMOVE(&freedHead, allocated_chunk, entry, entries);
    SLIST_INSERT_HEAD(&allocatedHead, allocated_chunk, entries);
    allocated_chunk->allocated_size = chunk_size;
    return allocated_chunk->data;
  }

  // Otherwise, allocate new memory block:

  // Considering this is a toy implementation of malloc, i'm assuming this dynamic allocation
  // is not considered a part of the memory "allocated" to the program. Other implementations
  // of linked lists would use dynamic allocation for "entries" or list nodes regardless.
  allocated_chunk = (struct entry *)sbrk(sizeof(struct entry));
  if (allocated_chunk == (void *)-1) {
    fprintf(stderr, "Error: Failed to allocate memory for entry\n");
    exit(EXIT_FAILURE);
  }

  // Allocate the requested block of memory
  allocated_chunk->data = sbrk(allocation_size);
  if (allocated_chunk->data == (void *)-1) {
    fprintf(stderr, "Error: Failed to allocate memory with sbrk\n");
    exit(EXIT_FAILURE);
  }

  allocated_chunk->allocated_size = chunk_size;
  allocated_chunk->size           = allocation_size;

  // Insert the new entry into the allocated list
  SLIST_INSERT_HEAD(&allocatedHead, allocated_chunk, entries);

  // Return the allocated block
  return allocated_chunk->data;
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

  // Early exit with error if allocated list is empty
  if (SLIST_EMPTY(&allocatedHead)) {
    printf("Error: No allocated memory to free\n");
    return;
  }

  /* Initially implemented to simplify FIFO deallocation
   *
   * // Deallocate head if NULL is passed
   * if (chunk == NULL) {
   *   struct entry *deallocmem = SLIST_FIRST(&allocatedHead);
   *   SLIST_REMOVE_HEAD(&allocatedHead, entries);
   *   SLIST_INSERT_HEAD(&freedHead, deallocmem, entries);
   * }
   */

  // Search for block to deallocate
  else {
    struct entry *deallocmem  = NULL;
    struct entry *searchedblk = NULL;

    // Traverse list for matching entry
    SLIST_FOREACH(deallocmem, &allocatedHead, entries) {
      if (deallocmem->data == chunk) {
        searchedblk = deallocmem;
        break;
      }
    }

    // Move block to deallocated list if matching entry is found
    if (searchedblk != NULL && searchedblk->data == chunk) {
      SLIST_REMOVE(&allocatedHead, searchedblk, entry, entries);
      SLIST_INSERT_HEAD(&freedHead, searchedblk, entries);
      searchedblk->allocated_size = 0;
    }

    // Otherwise, exit with error code
    else {
      fprintf(stderr, "Fatal Error: Attempting to deallocate invalid chunk\n");
      fprintf(stderr, "Exiting.\n");
      exit(EXIT_FAILURE);
    }
  }
}
