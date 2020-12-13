#ifndef lfs_h
#define lfs_h
#include "lfs_block.h"
#include "lfs_inode.h"
#include "lfs_segment.h"

#include "./prefix.h"
#include "./logger.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

struct SegBuffer
{
    struct Block blockdata[BLOCKS_PER_SEGMENT];
    struct Imap imap;
};

void lfs_init();
int lfs_create(struct stat st);
int lfs_read(const int fileId, char *buf, int offset, int size);
int lfs_write(const int fileId, const char *buf, int offset, int size);
int lfs_truncate(const int fileId, int size);
int lfs_remove(const int fileId);

void lfs_fflush();

int file_number();
int lfs_metadata(const int fileId, struct stat *st);
int lfs_change(const int fileId, struct stat st);

void lfs_save(); //
#endif