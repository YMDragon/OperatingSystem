#include "cache.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct CacheEntry cache[MAX_CACHE_ENTRY];
int FIFO = 0;
pthread_rwlock_t cacheLock = PTHREAD_RWLOCK_INITIALIZER;

int find_in_cache(int offset, int type)
{
    //pthread_rwlock_rdlock(&cacheLock);
    for (int i = 0; i < MAX_CACHE_ENTRY; i++)
        if (cache[i].valid == 1 && cache[i].offset == offset)
        {
            //printf("find %d in cache at %d\n", offset, i);
            if (type == 0)
            {
                //pthread_rwlock_rdlock(&cache[i].lock);}
            }
            else
            {
                //pthread_rwlock_wrlock(&cache[i].lock);
            }
            //pthread_rwlock_unlock(&cacheLock);
            return i;
        }
    //printf("do not find %d in cache\n", offset);
    //pthread_rwlock_unlock(&cacheLock);
    return -1;
}

int new_cache(int fd, int offset)
{
    //pthread_rwlock_wrlock(&cacheLock);
    if (cache[FIFO].valid == 1 && cache[FIFO].dirty == 1)
    {
        //printf("cache %d write_back\n", FIFO);
        pwrite(fd, cache[FIFO].block.data, BLOCKSIZE, cache[FIFO].offset);
        fsync(fd);
    }
    int pos = FIFO;
    //printf("put %d at cache %d\n", offset, pos);
    if (cache[pos].valid == 0)
    {
        //pthread_rwlock_init(&cache[pos].lock, NULL);
    }
    FIFO = (FIFO + 1) % (MAX_CACHE_ENTRY);
    cache[pos].offset = offset;
    cache[pos].valid = 1;
    //pthread_rwlock_wrlock(&cache[pos].lock);
    //pthread_rwlock_unlock(&cacheLock);
    cache[pos].dirty = 0;
    pread(fd, cache[pos].block.data, BLOCKSIZE, offset);
    return pos;
}

void cache_read(int fd, char *buff, int size, int offset)
{
    int endAddr = offset + size;
    int pos, end;
    int cache_id;
    for (pos = offset; pos < endAddr; pos = end)
    {
        end = MIN(pos / BLOCKSIZE * BLOCKSIZE + BLOCKSIZE, endAddr);
        if (end <= pos)
            break;
        cache_id = find_in_cache(pos, 0);
        if (cache_id == -1)
            cache_id = new_cache(fd, pos);
        memcpy(buff + pos - offset, cache[cache_id].block.data + pos % BLOCKSIZE, end - pos);
        //pthread_rwlock_unlock(&cache[cache_id].lock);
    }
}

void cache_write(int fd, char *buff, int size, int offset)
{
    int endAddr = offset + size;
    int pos, end;
    int cache_id;
    for (pos = offset; pos < endAddr; pos = end)
    {
        end = MIN(pos / BLOCKSIZE * BLOCKSIZE + BLOCKSIZE, endAddr);
        if (end <= pos)
            break;
        cache_id = find_in_cache(pos, 1);
        if (cache_id == -1)
            cache_id = new_cache(fd, pos);
        memcpy(cache[cache_id].block.data + pos % BLOCKSIZE, buff + pos - offset, end - pos);
        cache[cache_id].dirty = 1;
        //pthread_rwlock_unlock(&cache[cache_id].lock);
    }
}

void cache_writeback(int fd)
{
    //pthread_rwlock_wrlock(&cacheLock);
    for (int i = 0; i < MAX_CACHE_ENTRY; i++)
        if (cache[i].valid == 1 && cache[i].dirty == 1)
        {
            pwrite(fd, cache[FIFO].block.data, BLOCKSIZE, cache[FIFO].offset);
            fsync(fd);
        }
    //pthread_rwlock_unlock(&cacheLock);
}