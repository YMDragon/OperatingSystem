#include "chmod.h"

int o_chmod(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    logger(DEBUG, "CHMOD, %s\n", path);
    return -1;
}
