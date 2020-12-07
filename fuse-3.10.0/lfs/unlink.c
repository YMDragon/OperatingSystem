#include "unlink.h"

int o_unlink(const char *path)
{
    logger(DEBUG, "UNLINK, %s\n", path);
    logger(ERROR, "UNIMPLEMENTED: unlink, path: %s\n", path);
    return -1;
}
