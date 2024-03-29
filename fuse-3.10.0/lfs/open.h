#ifndef open_h
#define open_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include "directory.h"
#include "mutex.h"
int o_open(const char *, struct fuse_file_info *);
#endif
