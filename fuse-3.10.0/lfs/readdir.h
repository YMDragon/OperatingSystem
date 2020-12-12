#ifndef readdir_h
#define readdir_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_fill_dir_t fuse_file_info */
#include <sys/stat.h>
#include "directory.h"
int o_readdir(const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *, enum fuse_readdir_flags);
#endif
