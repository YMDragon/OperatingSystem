#ifndef permission_h
#define permission_h
#include "./lfs_inode.h"
#include <fuse.h>
#include <unistd.h>
#include <sys/stat.h>
int has_permission(struct stat st, int mode);
#endif