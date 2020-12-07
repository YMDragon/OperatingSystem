#include "ioctl.h"

int o_ioctl(const char *path, int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data)
{
    logger(DEBUG, "IOCTL, %s, %d, %#X, %d, %#X\n", path, cmd, arg, flags, data);
    logger(ERROR, "UNIMPLEMENTED: ioctl\n");
    return -1;
}
