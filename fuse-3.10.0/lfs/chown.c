#include "chown.h"

int o_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi)
{
    logger(DEBUG, "CHOWN, %s%s, %d, %d, %p\n", prefix, path, uid, gid, fi);
    logger(ERROR, "UNIMPLEMENTED: chown\n");
    return -1;
}
