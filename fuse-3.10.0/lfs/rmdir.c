#include "rmdir.h"

int o_rmdir(const char *path)
{
    logger(DEBUG, "RMDIR, %s\n", path);
    logger(ERROR, "UNIMPLEMENTED: rmdir, path: %s\n", path);
    return -1;
}
