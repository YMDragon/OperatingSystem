#ifndef fsyncdir_h
#define fsyncdir_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include "lfs.h"
#include "mutex.h"
int o_fsyncdir(const char *, int, struct fuse_file_info *);
#endif
