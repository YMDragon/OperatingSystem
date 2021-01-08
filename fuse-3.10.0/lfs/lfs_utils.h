#ifndef lfs_utils_h
#define lfs_utils_h
#include "lfs_block.h"
#include "lfs_inode.h"
#include "lfs_segment.h"

void _read_disk(char *buff, int offset, int size);

void _write_disk(char *buff, int offset, int size);

void _insert_inode(struct Inode *inode, int blockId, int blkNum);

void _clear_inode(struct Inode *inode);

int _fit_in_inode(struct Segment *segPos, struct Inode inode, int blkNum);

int _fit_in_imap(struct Imap imap, const int fileId);

struct Inode _get_inode_from_string(char *buff);

void _get_string_from_inode(struct Inode inode, char *buff);

struct Inode _fetch_inode(int fileId);

int _get_file_size(int fileId);

void garbage_collection();

void _rewrite(struct Segment *seg, int fileId, int blkNum);

int _is_new_available(struct Segment *seg, int fileId, int blockId);

void lfs_fflush();

#endif