#include "truncate.h"

int o_truncate(const char *path, off_t size)
{
    logger(DEBUG, "TRUNCATE, %s\n", path);
    return -1;
}
