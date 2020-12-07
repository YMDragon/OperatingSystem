#ifndef fallocate_h
#define fallocate_h
#include "./logger.h" /* logger */
#include <fuse.h>     /* fuse_file_info */
int o_fallocate(const char *, int, off_t, off_t, struct fuse_file_info *);
#endif
