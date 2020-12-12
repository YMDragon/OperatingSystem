#ifndef getattr_h
#define getattr_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <sys/stat.h> /* stat */
#include <fuse.h>     /* fuse_file_info */
#include "lfs.h"
#include "directory.h"
int o_getattr(const char *, struct stat *, struct fuse_file_info *);
#endif
