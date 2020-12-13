#ifndef chown_h
#define chown_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <unistd.h>   /* uid_t gid_t */
#include <fuse.h>     /* fuse_file_info */
#include "directory.h"
#include "lfs.h"
#include "permission.h"
int o_chown(const char *, uid_t, gid_t, struct fuse_file_info *);
#endif
