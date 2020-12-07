#ifndef write_buf_h
#define write_buf_h
#include "./logger.h" /* logger */
#include <fuse.h>     /* fuse_file_info */
int o_write_buf(const char *, struct fuse_bufvec *, off_t, struct fuse_file_info *);
#endif
