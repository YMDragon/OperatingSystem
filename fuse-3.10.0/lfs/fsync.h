#ifndef fsync_h
#define fsync_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_File_info */
#include "lfs.h"
#include "mutex.h"
int o_fsync(const char *, int, struct fuse_file_info *);
#endif
