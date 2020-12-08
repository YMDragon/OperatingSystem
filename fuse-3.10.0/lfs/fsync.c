#include "fsync.h"

int o_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
{
    logger(DEBUG, "FSYNC, %s%s, %d, %p\n", prefix, path, isdatasync, fi);
    logger(ERROR, "UNIMPLEMENTED: fsync\n");
    return -1;
}
