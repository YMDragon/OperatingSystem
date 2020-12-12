#ifndef release_h
#define release_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include "directory.h"
int o_release(const char *, struct fuse_file_info *);
#endif
