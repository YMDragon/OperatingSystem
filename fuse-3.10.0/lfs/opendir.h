#ifndef opendir_h
#define opendir_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include "directory.h"
int o_opendir(const char *, struct fuse_file_info *);
#endif
