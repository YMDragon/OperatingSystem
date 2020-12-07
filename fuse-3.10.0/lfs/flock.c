#include "flock.h"

int o_flock(const char *path, struct fuse_file_info *fi, int op)
{
    logger(DEBUG, "FLOCK, %s, %d\n", path, op);
    logger(ERROR, "UNIMPLEMENTED: flock\n");
    return -1;
}
