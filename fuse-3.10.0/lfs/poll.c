#include "poll.h"

int o_poll(const char *path, struct fuse_file_info *fi, struct fuse_pollhandle *ph, unsigned *reventsp)
{
    logger(DEBUG, "POLL, %s%s, %p, %p, %p\n", prefix, path, fi, ph, reventsp);
    logger(ERROR, "UNIMPLEMENTED: poll\n");
    return -1;
}
