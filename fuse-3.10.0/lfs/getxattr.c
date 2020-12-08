#include "getxattr.h"

int o_getxattr(const char *path, const char *name, char *value, size_t size)
{
    logger(DEBUG, "GETXATTR, %s%s, %s, %s, %d\n", prefix, path, name, value, size);
    logger(ERROR, "UNIMPLEMENTED: getxattr\n");
    return -1;
}
