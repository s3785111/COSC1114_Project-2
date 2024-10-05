#ifndef _LIBMEM_H
#define _LIBMEM_H

#define _DEFAULT_SOURCE

#include "stdio.h"
#include "sys/queue.h"

typedef enum {
  ALLOCATED,
  FREE
} AllocStatus;

struct memblk {
  size_t size;
  void *block;
};

struct entry {
  struct memblk data;
  SLIST_ENTRY(entry)
  entries;
};
SLIST_HEAD(slisthead, entry);

void printBlks(AllocStatus status);
void printBlk(AllocStatus status, struct entry *np);
void *alloc(size_t chunk_size);
void dealloc(void *chunk);

#endif
