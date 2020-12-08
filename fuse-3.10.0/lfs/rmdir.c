#include "rmdir.h"

int o_rmdir(const char *path)
{
    logger(DEBUG, "RMDIR, %s%s\n", prefix, path);
    logger(ERROR, "UNIMPLEMENTED: rmdir, path: %s\n", path);
    return -1;
}
