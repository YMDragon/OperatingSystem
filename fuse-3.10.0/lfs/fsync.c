#include "fsync.h"

int o_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
{
    logger(DEBUG, "FSYNC, %s%s, %d, %p\n", prefix, path, isdatasync, fi);
    lfs_fflush();
    return 0;
}
