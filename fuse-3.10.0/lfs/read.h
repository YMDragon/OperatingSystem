#ifndef read_h
#define read_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include <sys/stat.h>
#include "directory.h"
#include "permission.h"
#include "mutex.h"
int o_read(const char *, char *, size_t, off_t, struct fuse_file_info *);
#endif
