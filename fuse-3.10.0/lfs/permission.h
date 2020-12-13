#ifndef permission_h
#define permission_h
#include "./lfs_inode.h"
#include <fuse.h>
#include <unistd.h>
#include <sys/stat.h>
#include "lfs.h"
int has_permission(int fileId, int mode);
int is_owner(int fileId);
#endif