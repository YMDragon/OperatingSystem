#ifndef lseek_h
#define lseek_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
off_t o_lseek(const char *, off_t, int, struct fuse_file_info *);
#endif
