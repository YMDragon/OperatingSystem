#include "setxattr.h"

int o_setxattr(const char *path, const char *name, const char *value, size_t size, int flag)
{
    logger(DEBUG, "SETXATTR, %s%s, %s, %s, %d, %d\n", prefix, path, name, value, size, flag);
    logger(ERROR, "UNIMPLEMENTED: setxattr\n");
    return -1;
}
