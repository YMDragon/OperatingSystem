#ifndef releasedir_h
#define releasedir_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
#include "directory.h"
#include "mutex.h"
int o_releasedir(const char *, struct fuse_file_info *);
#endif
