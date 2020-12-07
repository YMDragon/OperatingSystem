#include "chown.h"

int o_chown(const char *path, uid_t uid, gid_t gid)
{
    logger(DEBUG, "CHOWN, %s\n", path);
    return -1;
}
