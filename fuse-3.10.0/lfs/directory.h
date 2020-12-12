#ifndef directory_h
#define directory_h
#include "lfs.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "permission.h"
#define ENTRYS_PER_DIRECTORY 16
struct DirectoryEntry
{
    char name[60];
    int fileId;
};
struct DirectoryBlock
{
    struct DirectoryEntry entry[ENTRYS_PER_DIRECTORY];
};
int rootId;
int add_entry(int fileId, const char *name, int size, int childfileId);
int create_dir(const char *name, int size, struct stat st, int parentfileId);
int get_fileId(const char *path);
int get_parent_fileId(const char *path, int *pos);
int is_match(struct DirectoryEntry *de, const char *name, int size);
#endif