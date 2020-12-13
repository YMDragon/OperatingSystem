#include "flush.h"

int o_flush(const char *path, struct fuse_file_info *fi)
{
    logger(DEBUG, "FLUSH, %s%s, %p\n", prefix, path, fi);
    return 0;
}
