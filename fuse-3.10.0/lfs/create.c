#include "create.h"

int o_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    logger(DEBUG, "CREATE, %s%s, %d, %p\n", prefix, path, mode, fi);
    logger(ERROR, "UNIMPLEMENTED: create\n");
    return -1;
}
