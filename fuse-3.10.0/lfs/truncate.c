#include "truncate.h"

int o_truncate(const char *path, off_t size, struct fuse_file_info *fi)
{
    logger(DEBUG, "TRUNCATE, %s%s, %d, %p\n", prefix, path, size, fi);
    logger(ERROR, "UNIMPLEMENTED: truncate\n");
    return -1;
}
