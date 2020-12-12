#ifndef inode_h
#define inode_h
#define MAX_BLOCKS_INODE 16
#include <sys/stat.h>
struct InodeEntry
{
    int blockId;
    int blkNum;
};
struct Inode
{
    struct stat st;
    //
    unsigned int atime, ctime, mtime;
    //
    unsigned int size;
    struct InodeEntry entry[MAX_BLOCKS_INODE];
    int level;
};
#endif