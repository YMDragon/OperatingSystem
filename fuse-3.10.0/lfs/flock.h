#ifndef flock_h
#define flock_h
#include "./logger.h" /* logger */
#include <fuse.h>     /* fuse_file_info */
int o_flock(const char *, struct fuse_file_info *, int op);
#endif
