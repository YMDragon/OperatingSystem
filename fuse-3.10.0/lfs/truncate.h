#ifndef truncate_h
#define truncate_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include "directory.h"
#include "lfs.h"
#include "mutex.h"
int o_truncate(const char *, off_t, struct fuse_file_info *);
#endif
