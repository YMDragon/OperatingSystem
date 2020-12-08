#ifndef chmod_h
#define chmod_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <sys/stat.h> /* mode_t */
#include <fuse.h>     /* fuse_file_info */
int o_chmod(const char *, mode_t, struct fuse_file_info *);
#endif
