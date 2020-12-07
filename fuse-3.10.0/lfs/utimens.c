#include "utimens.h"

int o_utimens(const char *path, const struct timespec ts[2])
{
    logger(DEBUG, "UTIMES, %s\n", path);
    return -1;
}
