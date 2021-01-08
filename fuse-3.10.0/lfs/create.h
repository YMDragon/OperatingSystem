#ifndef create_h
#define create_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <sys/stat.h> /* mode_t */
#include <fuse.h>     /* fuse_file_info */
#include "directory.h"
#include "lfs.h"
#include "mutex.h"
int o_create(const char *, mode_t, struct fuse_file_info *);
#endif
