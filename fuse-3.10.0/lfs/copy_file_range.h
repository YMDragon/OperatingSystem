#ifndef copy_file_range_h
#define copy_file_range_h
#include "./logger.h" /* logger */
#include <fuse.h>     /* fuse_file_info */
ssize_t o_copy_file_range(const char *, struct fuse_file_info *, off_t, const char *, struct fuse_file_info *, off_t, size_t, int);
#endif
