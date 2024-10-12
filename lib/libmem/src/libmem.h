#ifndef _LIBMEM_H
#define _LIBMEM_H

#define _DEFAULT_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <unistd.h>

typedef enum {
  ALLOCATED,
  FREE
} AllocStatus;

struct entry {
  size_t size;
  void *data;
  SLIST_ENTRY(entry)
  entries;
};
SLIST_HEAD(slisthead, entry);

void printBlks(AllocStatus status);
void printBlk(AllocStatus status, struct entry *np);
void *alloc(size_t chunk_size);
void dealloc(void *chunk);

struct entry *_getNextEntry(AllocStatus status, struct entry *entry);
__attribute__((weak)) void *strategy(size_t chunk_size);

#endif
