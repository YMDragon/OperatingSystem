#ifndef segment_h
#define segment_h
#include "lfs_block.h"
#define BLOCKS_PER_SEGMENT 1024
#define FILES_PER_SEGMENT 500
struct ImapEntry
{
    int fileId;
    int blkNum;
};
struct Imap
{
    struct ImapEntry entry[FILES_PER_SEGMENT];
    int numEntries;
};
struct Segment
{
    struct Segment *nextSegment;
    struct Imap imap;
    int filePos;
};
int segRead(struct Segment *segment, struct Block *block, int blkNum);
#endif