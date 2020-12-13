#ifndef directory_h
#define directory_h
#include "lfs.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "permission.h"
struct DirectoryEntry
{
    char name[252];
    int fileId;
};
int rootId;
int add_entry(int fileId, const char *name, int size, int childfileId);
int create_dir(const char *name, int size, struct stat st, int parentfileId);
int get_fileId(const char *path);
int get_parent_fileId(const char *path, int *pos);
int is_match(struct DirectoryEntry *de, const char *name, int size);
#endif