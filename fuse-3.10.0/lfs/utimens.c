#include "utimens.h"

int o_utimens(const char *path, const struct timespec ts[2], struct fuse_file_info *fi)
{
    logger(DEBUG, "UTIMES, %s\n", path);
    return -1;
}
