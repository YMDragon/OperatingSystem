#ifndef init_h
#define init_h
#include "./logger.h" /* logger */
#include <fuse.h>     /* fuse_conn_info */
#include <sys/stat.h>
#include "lfs.h"
#include "directory.h"
#include "mutex.h"
void *o_init(struct fuse_conn_info *, struct fuse_config *cfg);
#endif
