#include "fsyncdir.h"

int o_fsyncdir(const char *path, int isdatasync, struct fuse_file_info *fi)
{
    logger(DEBUG, "FSYNCDIR, %s%s, %d, %p\n", prefix, path, isdatasync, fi);
    lfs_fflush();
    return 0;
}
