#include "create.h"

int o_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    logger(DEBUG, "CREATE, %s, %d\n", path, mode);
    logger(ERROR, "UNIMPLEMENTED: create\n");
    return -1;
}
