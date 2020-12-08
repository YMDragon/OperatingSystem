#include "access.h"

int o_access(const char *path, int mode)
{
    logger(DEBUG, "ACCESS, %s%s, %d\n", prefix, path, mode);
    logger(ERROR, "UNIMPLEMENTED: access, path: %s, mode: %d\n", path, mode);
    return -1;
}
