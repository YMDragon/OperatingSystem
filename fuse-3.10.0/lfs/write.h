#ifndef write_h
#define write_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include <sys/stat.h>
#include "directory.h"
#include "permission.h"
#include "mutex.h"
int o_write(const char *, const char *, size_t, off_t, struct fuse_file_info *);
#endif
