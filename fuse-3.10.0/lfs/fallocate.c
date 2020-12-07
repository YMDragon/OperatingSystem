#include "fallocate.h"

int o_fallocate(const char *path, int mode, off_t offset, off_t length, struct fuse_file_info *fi)
{
    logger(DEBUG, "FALLOCATE, %s, %d, %d, %d\n", path, mode, offset, length);
    logger(ERROR, "UNIMPLEMENTED: fallocate\n");
    return -1;
}
