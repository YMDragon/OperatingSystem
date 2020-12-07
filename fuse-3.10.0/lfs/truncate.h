#ifndef truncate_h
#define truncate_h
#include "./logger.h" /* logger */
#include <fuse.h>     /* fuse_file_info */
int o_truncate(const char *, off_t, struct fuse_file_info *);
#endif
