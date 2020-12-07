#include "chown.h"

int o_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi)
{
    logger(DEBUG, "CHOWN, %s\n", path);
    return -1;
}
