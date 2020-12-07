#include "chmod.h"

int o_chmod(const char *path, mode_t mode)
{
    logger(DEBUG, "CHMOD, %s\n", path);
    return -1;
}
