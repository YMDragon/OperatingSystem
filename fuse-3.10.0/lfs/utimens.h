#ifndef utimens_h
#define utimens_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include "directory.h"
#include "mutex.h"
int o_utimens(const char *, const struct timespec[2], struct fuse_file_info *);
#endif
