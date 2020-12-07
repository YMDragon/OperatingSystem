#ifndef read_buf_h
#define read_buf_h
#include "./logger.h" /* logger */
#include <fuse.h>     /* fuse_file_info */
int o_read_buf(const char *, struct fuse_bufvec **, size_t, off_t, struct fuse_file_info *);
#endif
