#include "chmod.h"

int o_chmod(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    logger(DEBUG, "CHMOD, %s%s, %d, $p\n", prefix, path, mode, fi);
    logger(ERROR, "UNIMPLEMENTED: chmod\n");
    return -1;
}
