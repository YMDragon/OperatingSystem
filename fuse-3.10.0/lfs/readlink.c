#include "readlink.h"

int o_readlink(const char *path, char *buf, size_t size)
{
    logger(DEBUG, "READLINK, %s%s, %s, %d\n", prefix, path, buf, size);
    logger(ERROR, "UNIMPLEMENTED: readlink, path: %s, buf: %s, size: %d\n", path, buf, size);
    return -1;
}
