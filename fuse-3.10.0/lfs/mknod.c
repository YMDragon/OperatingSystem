#include "mknod.h"

int o_mknod(const char *path, mode_t mode, dev_t rdev)
{
    logger(DEBUG, "MKNOD, %s%s, %d, %d\n", prefix, path, mode, rdev);
    logger(ERROR, "UNIMPLEMENTED: mknod, path: %s, mode: %d, rdev: %d\n", path, mode, rdev);
    return -1;
}
