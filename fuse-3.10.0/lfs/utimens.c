#include "utimens.h"

int o_utimens(const char *path, const struct timespec ts[2], struct fuse_file_info *fi)
{
    logger(DEBUG, "UTIMES, %s%s, %p, %p\n", prefix, path, ts, fi);
    logger(ERROR, "UNIMPLEMENTED: utimens\n");
    return -1;
}
