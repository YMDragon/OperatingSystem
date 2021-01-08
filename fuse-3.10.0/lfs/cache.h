#ifndef cache_h
#define cache_h
#include "lfs_block.h"
#include <pthread.h>

#define MAX_CACHE_ENTRY (2 * 1024)

struct CacheEntry
{
    struct Block block;
    int offset;
    int valid;
    int dirty;
    pthread_rwlock_t lock;
};

void cache_read(int fd, char *buff, int size, int offset);
void cache_write(int fd, char *buff, int size, int offset);
void cache_writeback(int fd);

#endif