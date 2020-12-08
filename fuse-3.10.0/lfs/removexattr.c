#include "removexattr.h"

int o_removexattr(const char *path, const char *name)
{
    logger(DEBUG, "REMOVEXATTR, %s%s, %s\n", prefix, path, name);
    logger(ERROR, "UNIMPLEMENTED: removexattr\n");
    return -1;
}
